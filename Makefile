KERNEL_SRC  = kernel/src
DRIVER_DIR  = kernel/src/drivers
CORE_DIR    = kernel/src/core
INC_DIR     = include
BIN_DIR     = bin
ISODIR      = isodir
CC          = gcc
CFLAGS      = -ffreestanding -O2 -nostdlib -mcmodel=kernel -I$(INC_DIR) -mno-red-zone
ASM_SRC := $(shell find $(KERNEL_SRC) -name "*.asm")
ASM_OBJ := $(patsubst %.asm,$(BIN_DIR)/%.o,$(ASM_SRC))

SRC := $(shell find $(KERNEL_SRC) -name "*.c")
OBJ := $(patsubst %.c,$(BIN_DIR)/%.o,$(SRC))

DIRS := $(sort $(dir $(OBJ)))

all: build

$(BIN_DIR)/%/:
	mkdir -p $@

$(BIN_DIR)/%.o: %.c | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: %.asm | $(DIRS)
	nasm -f elf64 $< -o $@

build: $(OBJ) $(ASM_OBJ)
	$(CC) -T $(KERNEL_SRC)/linker.ld $(OBJ) $(ASM_OBJ) -o $(BIN_DIR)/kernel.bin -lgcc -nostdlib

iso: build
	cp $(BIN_DIR)/kernel.bin boot/EFI/limine/boot/kernel.bin

	xorriso -as mkisofs -R -r -J -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		boot/EFI/limine -o $(BIN_DIR)/kernel.iso

	limine bios-install $(BIN_DIR)/kernel.iso

run: iso
	qemu-system-x86_64 -cdrom bin/kernel.iso \
		-m 2048

clean:
	rm -rf $(BIN_DIR)
