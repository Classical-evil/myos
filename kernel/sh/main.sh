gcc-4.4 ~/c9/kernel/main.c -o ~/c9/kernel/main.o -c -m32
ld ~/c9/kernel/main.o -Ttext 0xc0001500 -e main -o ~/c9/kernel/kernel.bin -m elf_i386
dd if=~/c9/kernel/kernel.bin of=/home/cooiboi/bochs1/hd60M.img bs=512 count=200 seek=9 conv=notrunc
