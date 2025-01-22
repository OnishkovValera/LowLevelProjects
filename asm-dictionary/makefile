ASM = nasm -f elf64
PYTHON = python3
OBJ_DIR = obj
SRC_FILES = $(wildcard *.asm)
OBJECT_FILES = $(patsubst %.asm, $(OBJ_DIR)/%.o, $(SRC_FILES))

program: $(OBJECT_FILES)
	ld $(OBJECT_FILES) -o labfile


$(OBJ_DIR)/%.o: %.asm | $(OBJ_DIR)
	$(ASM) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean: 
	rm -rf $(OBJ_DIR) program
	rm labfile
test:
	$(PYTHON) test.py
.PHONY: clean test
