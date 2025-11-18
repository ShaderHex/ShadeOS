KERNEL_SRC  = kernel/src
DRIVER_DIR  = kernel/src/drivers
CORE_DIR    = kernel/src/core
INC_DIR     = include
BIN_DIR     = bin
ISODIR      = isodir

CC          = gcc
CFLAGS      = -ffreestanding -O2 -nostdlib -mcmodel=kernel -I$(INC_DIR)

# Automatically find all .c files
SRC := $(shell find $(KERNEL_SRC) -name "*.c")
# Convert each .c file into a .o output in bin/
OBJ := $(patsubst %.c,$(BIN_DIR)/%.o,$(SRC))

# Make sure folders inside bin/ exist
DIRS := $(sort $(dir $(OBJ)))

all: build

# Create bin structure mirroring src structure
$(BIN_DIR)/%/:
	mkdir -p $@

# Compile rule for each object
$(BIN_DIR)/%.o: %.c | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Build the kernel
build: $(OBJ)
	$(CC) -T $(KERNEL_SRC)/linker.ld $(OBJ) -o $(BIN_DIR)/kernel.bin -lgcc -nostdlib

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
