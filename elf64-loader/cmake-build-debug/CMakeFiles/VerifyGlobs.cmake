# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.29
cmake_policy(SET CMP0009 NEW)

# test_directories at tester/CMakeLists.txt:1 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/*")
set(OLD_GLOB
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/.gitignore"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/1"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/2"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/3"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/4"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/5"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/6"
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/tester/tests/7"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()

# sources at solution/CMakeLists.txt:1 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/solution/include/*.h")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()

# sources at solution/CMakeLists.txt:1 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/solution/src/*.c")
set(OLD_GLOB
  "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/solution/src/main.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()

# sources at solution/CMakeLists.txt:1 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/solution/src/*.h")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/onish/Итмо/ВС/assignment-1-elf64-loader/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()
