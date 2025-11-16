KERNEL_SRC	= kernel/src
DRIVER_DIR 	= kernel/src/drivers
BIN_DIR		= bin
ISODIR		= isodir
INC 		= include
CC 		    = gcc
C_FLAGS		= -ffreestanding -O2 -nostdlib -mcmodel=kernel
INC_DIR 	= include/
all: build

rmbin:
	rm -rf bin
mkbin: rmbin
	mkdir -p bin
build: mkbin
	${CC} -c ${C_FLAGS} -I${INC_DIR} ${KERNEL_SRC}/kernel.c -o ${BIN_DIR}/kernel.o
	${CC} -c ${C_FLAGS} -I${INC_DIR} ${DRIVER_DIR}/display.c -o ${BIN_DIR}/display.o
	${CC} -c ${C_FLAGS} -I${INC_DIR} ${DRIVER_DIR}/io.c -o ${BIN_DIR}/io.o
	${CC} -c ${C_FLAGS} -I${INC_DIR} ${DRIVER_DIR}/display.c -o ${BIN_DIR}/display.o
	${CC} -T ${KERNEL_SRC}/linker.ld \
		${BIN_DIR}/kernel.o ${BIN_DIR}/display.o ${BIN_DIR}/io.o -o ${BIN_DIR}/kernel.bin \
		-lgcc -nostdlib
iso: build
	cp ${BIN_DIR}/kernel.bin boot/EFI/limine/boot/kernel.bin

	xorriso -as mkisofs -R -r -J -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        boot/EFI/limine -o ${BIN_DIR}/kernel.iso

	limine bios-install ${BIN_DIR}/kernel.iso
run: iso
	qemu-system-x86_64 -cdrom bin/kernel.iso \
		-no-reboot \
		-m 2048
