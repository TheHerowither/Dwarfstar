all: dwarfstar

dwarfstar: kernel bootloader
	ld -T link.ld -o bin/dwarfstar.bin bin/boot.o bin/kernel.o
	mkdir -p dwarfstar/boot/grub
	cp bin/dwarfstar.bin dwarfstar/boot/dwarfstar.bin
	cp grub.cfg dwarfstar/boot/grub/grub.cfg
	grub-mkrescue -o dwarfstar.iso dwarfstar

kernel: kernel.c bin
	gcc -c kernel.c -o ./bin/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-pie -fno-stack-protector -fno-builtin -nostdinc

bootloader: boot.asm bin
	nasm -felf64 boot.asm -o bin/boot.o

run: dwarfstar
	qemu-system-x86_64 -drive format=raw,file=dwarfstar.iso

bin:
	mkdir $@
