# ELF64 file loader
---
Загрузка ELF64 файла

# Подготовка

- Документация по формату PE файла: https://www.uclibc.org/docs/elf-64-gen.pdf
- правила MISRA C, доступне начиная со страницы 12 в http://easyelectronics.ru/files/Book/misra_c_rus.pdf

# Проект

Реализовать чтение и загрузку в виртуальную память файла в формате ELF64 и передачу управления на стартовый адрес указанной секции.
Формат использования утилиты следующий:

```C
./elf64-loader <source-elf64-file> <section-name>
```

Аргументы:
  - `<source-elf64-file>` - имя файла в формате ELF64
  - `<section-name>` - имя секции, стартовому адресу которой необходимо передать управление после загрузки ELF64 файла в виртуальную память.

Требования к реализации:
  - Код размещается в директории `solution/src`, заголовочные файлы ищутся в `solution/include`;
  - Ввиду требований к реализации, необходима поддержка GNU-совместимых компиляторов;
  - Грузиться будут только загружамые данные, хранящиеся в файле ELF64;
  - Позиционно-независимый (PIC) код из ELF64 не поддерживается;
  - Динамические библиотеки не поддерживаются;
  - Сообщения об ошибках выводятся только в стандартный поток ошибок (`stderr` равный 2);
  - В стандартный поток вывода утилита не должна выводить ничего (но можно предусмотреть ключ `-v` для диагностики);
  - В случае возникновения ошибки в ходе выполнения прогарммы, ошибку следует обработать, и, если выполнение продолжить невозможно, то надо возвращать не какое-то ненулевое число, а один из стандартных кодов ошибки:
    - Стандартные коды ошибки можно посмотеть в `/usr/include/asm-generic/errno-base.h`;

# ELF64 файл

ELF64 файлы используются в ОС Linux для хранения объектных и исполняемых файлов. Все заголовки ELF64 файлов доступны в заголовочном файле `elf.h`.

Формат ELF64 файла достаточно простой. Файл состоит из нескольих заголовков и данных, которые должны быть загружены в соответствующие адреса виртуальной памяти.

## Заголовок файла

Файл ELF64 начинается с заголовка, содержащего основную информацию о файле (все эти структуры и константы определены в заголовочном файле `elf.h`):

```C
typedef struct
{
  unsigned char e_ident[16]; /* ELF identification */
  Elf64_Half e_type;         /* Object file type */
  Elf64_Half e_machine;      /* Machine type */
  Elf64_Word e_version;      /* Object file version */
  Elf64_Addr e_entry;        /* Entry point address */
  Elf64_Off  e_phoff;        /* Program header offset */
  Elf64_Off  e_shoff;        /* Section header offset */
  Elf64_Word e_flags;        /* Processor-specific flags */
  Elf64_Half e_ehsize;       /* ELF header size */
  Elf64_Half e_phentsize;    /* Size of program header entry */
  Elf64_Half e_phnum;        /* Number of program header entries */
  Elf64_Half e_shentsize;    /* Size of section header entry */
  Elf64_Half e_shnum;        /* Number of section header entries */
  Elf64_Half e_shstrndx;     /* Section name string table index */
} Elf64_Ehdr;
```

После чтения заголовка обязательным условием является проверка magic number в поле идентификатора (`e_ident`). Первые четыре байла должны равняться `\x7f`, 
`E`, `L` и `F` соответственно.

В данном заголовке вас больше всего вас будут интересовать следующие поля:
- `e_phoff` - смещение на первый из программных заголовков;
- `e_phnum` - количество программных заголовков;
- `e_shoff` - смещение на первый заголовок секции;
- `e_shnum` - количество заголовков секций;
- `e_shstrndx` - индекс секции с таблицой имен секций.

## Программный заголовок (Program header)

Программные заголовки находятся начиная со смещения `e_phoff`, храняшегося в основном заголовке ELF64, и содержат информацию об областях виртуальной памяти и что в них надо загрузить:

```C
typedef struct
{
  Elf64_Word  p_type;   /* Type of segment */
  Elf64_Word  p_flags;  /* Segment attributes */
  Elf64_Off   p_offset; /* Offset in file */
  Elf64_Addr  p_vaddr;  /* Virtual address in memory */
  Elf64_Addr  p_paddr;  /* Reserved */
  Elf64_Xword p_filesz; /* Size of segment in file */
  Elf64_Xword p_memsz;  /* Size of segment in memory */
  Elf64_Xword p_align;  /* Alignment of segment */
} Elf64_Phdr;
```


Для выделения сегмента в виртуальной памяти используем функцию `mmap`, для установки прав на сегмент после загрузки данных используем функция `mprotect`. 


## Заголовок секции (Section header)

Заголовки секций начинаются со смещения `e_shoff` из заголовка ELF64 файлв. Они содержат информацию об секциях и привязку секций как к виртуальной памяти, так и к файлу:

```C
typedef struct
{
  Elf64_Word  sh_name;      /* Section name */
  Elf64_Word  sh_type;      /* Section type */
  Elf64_Xword sh_flags;     /* Section attributes */
  Elf64_Addr  sh_addr;      /* Virtual address in memory */
  Elf64_Off   sh_offset;    /* Offset in file */
  Elf64_Xword sh_size;      /* Size of section */
  Elf64_Word  sh_link;      /* Link to other section */
  Elf64_Word  sh_info;      /* Miscellaneous information */
  Elf64_Xword sh_addralign; /* Address alignment boundary */
  Elf64_Xword sh_entsize;   /* Size of entries, if section has table */
} Elf64_Shdr;
```

## Таблица имён (Section name string table)

Таблица имен находится по смещению секции под индексом `e_shstrndx` из основного заголовка файлов. Таблица имен содержит нуль-терминированые строки с именами.

