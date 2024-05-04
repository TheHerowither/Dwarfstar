all: dwarfstar

dwarfstar: kernel bootloader
	@echo Linking the kernel
	@ld -T link.ld -o bin/dwarfstar.bin bin/boot.o bin/kernel.o -melf_i386
	@echo [INFO]: Checking for valid multiboot header
	@if grub-file --is-x86-multiboot bin/dwarfstar.bin; then \
		echo [INFO]: Dwarfstar OS has a valid multiboot header ;\
	else \
		echo [ERROR]: Dwarfstar OS is not multiboot ;\
		echo [ERROR]: Binary did not compile properly ;\
	fi
	@grub-file --is-x86-multiboot bin/dwarfstar.bin
	@echo [INFO]: Creating bootable .iso file
	@mkdir -p dwarfstar/boot/grub
	@cp bin/dwarfstar.bin dwarfstar/boot/dwarfstar.bin
	@cp grub.cfg dwarfstar/boot/grub/grub.cfg
	@grub-mkrescue -o dwarfstar.iso dwarfstar

kernel: kernel.c bin
	@gcc -m32 -ffreestanding -fno-pie -c kernel.c -o bin/kernel.o

bootloader: boot.asm bin
	@nasm -felf32 boot.asm -o bin/boot.o

run: dwarfstar
	qemu-system-i386 -drive format=raw,file=dwarfstar.iso

bin:
	@mkdir $@
