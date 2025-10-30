all: run

OBJS = kernel.o kernel-entry.o \
       drivers/display.o drivers/keyboard.o core/software/shell.o

kernel.bin: $(OBJS)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: $(OBJS)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

kernel-entry.o: kernel-entry.asm
	nasm $< -f elf -o $@

%.o: %.c
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@

mbr.bin: mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > os-image.bin
	dd if=/dev/zero bs=512 count=10 >> os-image.bin 2>/dev/null || true

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

debug: os-image.bin kernel.elf
	qemu-system-i386 -S -s -fda os-image.bin &
	i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	$(RM) *.bin *.o *.elf
