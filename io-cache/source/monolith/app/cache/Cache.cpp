#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#define BLOCK_SIZE 4096
#define CACHE_CAPACITY 4

using CacheKey = struct {
  dev_t st_dev;
  ino_t st_ino;
};

using CacheBlock = struct {
  off_t offset;
  char* data;
  size_t valid_size;
  int is_dirty;
  int fd;
  CacheKey key;
};

using Cache = struct {
  std::vector<CacheBlock*> blocks;
  size_t size;
  pthread_mutex_t lock;
  std::mt19937 rng;
};

static Cache cache = {std::vector<CacheBlock*>(), 0, PTHREAD_MUTEX_INITIALIZER, std::mt19937(std::random_device{}())};

static CacheBlock* find_block(int fd, off_t offset);
static void evict_if_needed();
static int delete_block(CacheBlock* block);
static void add_block(CacheBlock* block);
static int load_block(int fd, off_t offset, CacheBlock** block);

int lab2_open(const char* path) {
    int fd = open(path, O_RDWR | O_DIRECT);
    if (fd == -1) {
        perror("lab2_open: failed to open file");
        return -1;
    }
    return fd;
}

int lab2_close(int fd) {
    pthread_mutex_lock(&cache.lock);
    auto it = cache.blocks.begin();
    while (it != cache.blocks.end()) {
        CacheBlock* block = *it;
        if (block->fd == fd) {
            if (delete_block(block) == -1) {
            perror("lab2_close: failed to delete block");
            pthread_mutex_unlock(&cache.lock);
            return -1;
            }
            it = cache.blocks.erase(it);
        }else {
            ++it;
        }
    }

    pthread_mutex_unlock(&cache.lock);
    return close(fd);
}

ssize_t lab2_read(int fd, void* buf, size_t count) {
    pthread_mutex_lock(&cache.lock); 

    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == static_cast<off_t>(-1)) {
        perror("lab2_read: failed to get current offset");
        pthread_mutex_unlock(&cache.lock);
        return -1;
    }

    size_t bytes_read = 0;

    while (count > 0) {
        off_t block_offset = (offset / BLOCK_SIZE) * BLOCK_SIZE;
        size_t block_offset_within = offset % BLOCK_SIZE;
        size_t bytes_to_copy = BLOCK_SIZE - block_offset_within;

        bytes_to_copy = std::min(bytes_to_copy, count);

        CacheBlock* block = find_block(fd, block_offset);

        if (block == nullptr) {
            int read_bytes = load_block(fd, block_offset, &block);
            if (read_bytes == -1) {
                perror("lab2_read: failed to load block");
                pthread_mutex_unlock(&cache.lock);
                return -1;
            }

            if (read_bytes == 0) {
                break;
            }

            block->is_dirty = 0;
            add_block(block);
        }

        size_t valid_bytes = std::min(bytes_to_copy, block->valid_size - block_offset_within);

        if (valid_bytes == 0) { 
            break;
        }

        memcpy(static_cast<char*>(buf) + bytes_read, block->data + block_offset_within, valid_bytes);

        bytes_read += valid_bytes;
        count -= valid_bytes;
        offset += valid_bytes;

        if (valid_bytes < bytes_to_copy) {
            break;
        }
    }

    pthread_mutex_unlock(&cache.lock); 
    return bytes_read;
}



ssize_t lab2_write(int fd, const void* buf, size_t count) {
    pthread_mutex_lock(&cache.lock);
    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == static_cast<off_t>(-1)) {
        perror("lab2_write: failed to get current offset");
        pthread_mutex_unlock(&cache.lock);
        return -1;
    }
    size_t bytes_written = 0;

    while (count > 0) {
        off_t block_offset = (offset / BLOCK_SIZE) * BLOCK_SIZE;
        size_t block_offset_within = offset % BLOCK_SIZE;
        size_t bytes_to_copy = BLOCK_SIZE - block_offset_within;
        bytes_to_copy = std::min(bytes_to_copy, count);

        CacheBlock* block = find_block(fd, block_offset);

        if (block == nullptr) {
            int read_bytes = load_block(fd, block_offset, &block);
            if (read_bytes == -1) {
                perror("lab2_write: failed to load block");
                pthread_mutex_unlock(&cache.lock);
                return -1;
            }
            if (read_bytes == 0) {
                break;
            }
            block->is_dirty = 1;
            add_block(block);
        } else {
            block->is_dirty = 1;
        }

        memcpy(block->data + block_offset_within, (char*)buf + bytes_written, bytes_to_copy);
        size_t new_end = block_offset_within + bytes_to_copy;
        block->valid_size = std::max(new_end, block->valid_size);

        bytes_written += bytes_to_copy;
        count -= bytes_to_copy;
        offset += bytes_to_copy;
    }
    pthread_mutex_unlock(&cache.lock);
    return bytes_written;
}

off_t lab2_lseek(int fd, off_t offset, int whence) {
    return lseek(fd, offset, whence);
}

int lab2_fsync(int fd) {
    pthread_mutex_lock(&cache.lock);

    for (auto& block : cache.blocks) {
        if (block->fd == fd && (block->is_dirty != 0)) {
            if (pwrite(fd, block->data, block->valid_size, block->offset) == -1) {
                perror("lab2_fsync: failed to write dirty block");
                pthread_mutex_unlock(&cache.lock);
                return -1;
            }
            block->is_dirty = 0;
        }
  }

  pthread_mutex_unlock(&cache.lock);
  return fsync(fd);
}

static CacheBlock* find_block(int fd, off_t offset) {
    for (auto& block : cache.blocks) {
        if (block->fd == fd && block->offset == offset) {
        return block;
        }
    }
    return nullptr;
}

static int delete_block(CacheBlock* block) {
  if (block->is_dirty != 0) {
    if (pwrite(block->fd, block->data, block->valid_size, block->offset) == -1) {
      perror("delete_block: failed to write dirty block");
      return -1;
    }
  }
  free(block->data);
  free(block);
  cache.size--;
  return 0;
}

static void evict_if_needed() {
  while (cache.size >= CACHE_CAPACITY) {
    std::uniform_int_distribution<> dist(0, cache.blocks.size() - 1);
    size_t index = dist(cache.rng);
    CacheBlock* block = cache.blocks[index];
    if (delete_block(block) == -1) {
      perror("evict_if_needed: failed to delete block");
    }
    cache.blocks.erase(cache.blocks.begin() + index);
  }
}

static void add_block(CacheBlock* block) {
  evict_if_needed();
  cache.blocks.push_back(block);
  cache.size++;
}

static int load_block(int fd, off_t block_offset, CacheBlock** block) {
    *block = static_cast<CacheBlock*>(malloc(sizeof(CacheBlock)));
    if ((*block) == nullptr) {
        perror("load_block: failed to allocate memory for block");
        return -1;
    }

    (*block)->data = static_cast<char*>(aligned_alloc(BLOCK_SIZE, BLOCK_SIZE));
    if ((*block)->data == nullptr) {
        perror("load_block: failed to allocate aligned memory");
        free(*block);
        return -1;
    }

    (*block)->offset = block_offset;
    (*block)->fd = fd;

    ssize_t read_bytes = pread(fd, (*block)->data, BLOCK_SIZE, block_offset);
    if (read_bytes < 0) { 
        perror("load_block: pread failed");
        free((*block)->data);
        free(*block);
        return -1;
    }

    if (read_bytes == 0) {
        free((*block)->data);
        free(*block);
        return 0;
    }

    (*block)->valid_size = read_bytes; 
    (*block)->is_dirty = 0;
    return read_bytes;
}

