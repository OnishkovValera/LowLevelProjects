#include "mem.h"
#include "mem_internals.h"

#include <assert.h>
#include <stdio.h>

#define HEAP_SIZE 128

static void allocation_success_test(){
    void *heap = heap_init(HEAP_SIZE);

    debug_heap(stdout, heap);
    assert(heap != NULL);

    heap_term();
}


static void single_block_released_test(){
    void *heap = heap_init(HEAP_SIZE);
    assert(heap != NULL);

    debug_heap(stdout, heap);

    void *mem_block = _malloc(64);

    assert(mem_block != NULL);
    debug_heap(stdout, heap);

    _free(mem_block);

    debug_heap(stdout, heap);
    heap_term();
}

static void two_blocks_released_test(){

    void *heap = heap_init(HEAP_SIZE);

    assert(heap != NULL);
    debug_heap(stdout, heap);

    void *mem_block1 = _malloc(32);
    void *mem_block2 = _malloc(64);

    assert(mem_block1 != NULL && mem_block2 != NULL);
    debug_heap(stdout, heap);

    _free(mem_block2);
    _free(mem_block1);

    debug_heap(stdout, heap);
    heap_term();
}


static void region_extension_test() {

    void *heap = heap_init(HEAP_SIZE);
    debug_heap(stdout, heap);

    void *mem_block1 = _malloc(64);
    assert(mem_block1 != NULL);
    debug_heap(stdout, heap);

    void *mem_block2 = _malloc(HEAP_SIZE * 4);
    assert(mem_block2 != NULL);
    debug_heap(stdout, heap);

    _free(mem_block1);
    _free(mem_block2);
    debug_heap(stdout, heap);
    heap_term();
}

static void region_extension_second_test() {

    void *heap = heap_init(HEAP_SIZE);
    debug_heap(stdout, heap);

    void *mem_block1 = _malloc(64);
    assert(mem_block1 != NULL);
    debug_heap(stdout, heap);

    void *mem_block2 = _malloc(HEAP_SIZE * 2);
    assert(mem_block2 != NULL);
    debug_heap(stdout, heap);

    _free(mem_block1);

    void *mem_block3 = _malloc(HEAP_SIZE * 4);
    assert(mem_block2 != NULL);
    debug_heap(stdout, heap);

    _free(mem_block2);
    _free(mem_block3);

    heap_term();
}

int main() {

    allocation_success_test();
    single_block_released_test();
    two_blocks_released_test();
    region_extension_test();
    region_extension_second_test();

    return 0;
}
