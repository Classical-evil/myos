nasm -o ~/c9/c2/mbr.bin ~/c9/c2/mbr.S
nasm -I ~/c9/c2/include/ -o ~/c9/c2/loader.bin ~/c9/c2/loader.S


dd if=~/c9/c2/mbr.bin    of=/home/cooiboi/bochs1/hd60M.img bs=512 count=1 conv=notrunc
dd if=~/c9/c2/loader.bin of=/home/cooiboi/bochs1/hd60M.img bs=512 count=4 seek=2 conv=notrunc
