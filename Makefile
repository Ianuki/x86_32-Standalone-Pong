# Toolchain
AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -m32
LDFLAGS = -T linker.ld --oformat binary

SRC_DIR = source
BIN_DIR = bin

BOOT_BIN = $(BIN_DIR)/boot.bin
MAIN_BIN = $(BIN_DIR)/main.bin
OS_IMAGE = $(BIN_DIR)/run.bin

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(C_SOURCES))

all: $(OS_IMAGE)

$(BOOT_BIN): $(SRC_DIR)/boot.asm
	@mkdir -p $(BIN_DIR)
	$(AS) -f bin $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_BIN): $(C_OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

$(OS_IMAGE): $(BOOT_BIN) $(MAIN_BIN)
	cat $(BOOT_BIN) $(MAIN_BIN) > $(OS_IMAGE)
	truncate -s 10k $(OS_IMAGE) 
	@echo "-> $(OS_IMAGE)"

clean:
	rm -f $(BIN_DIR)/*

run: all
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

.PHONY: all clean run
