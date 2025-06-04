BOCHS=/home/cooiboi/bochs1
CC=gcc-4.4
INCLUDE=-I lib/kernel/ -I kernel/ -I device/ -I thread/ 
CFLAGS=$(INCLUDE) -m32 -c -fno-builtin 
ASM=nasm
ASFLAGS=-f elf
DIR=./build
LD=ld
LDFLAGS=-m elf_i386 -Ttext 0xc0001500 -e main
OBJS=$(DIR)/main.o  $(DIR)/debug.o $(DIR)/init.o $(DIR)/interrupt.o $(DIR)/timer.o $(DIR)/kernel.o $(DIR)/print.o $(DIR)/string.o $(DIR)/bitmap.o $(DIR)/memory.o $(DIR)/thread.o $(DIR)/list.o $(DIR)/switch.o 



$(DIR)/main.o:kernel/main.c lib/kernel/print.h lib/kernel/stdint.h kernel/init.h 
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/timer.o:device/timer.c device/timer.h lib/kernel/stdint.h lib/kernel/io.h lib/kernel/print.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/interrupt.o:kernel/interrupt.c kernel/interrupt.h lib/kernel/stdint.h kernel/global.h lib/kernel/io.h lib/kernel/print.h 
	$(CC) $(CFLAGS) -o $@ $<


$(DIR)/init.o:kernel/init.c  kernel/init.h lib/kernel/print.h lib/kernel/stdint.h kernel/interrupt.h device/timer.h  
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/debug.o:kernel/debug.c kernel/debug.h lib/kernel/print.h lib/kernel/stdint.h kernel/interrupt.h  
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/string.o:lib/kernel/string.c lib/kernel/string.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/bitmap.o:kernel/bitmap.c kernel/bitmap.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/memory.o:kernel/memory.c kernel/memory.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/thread.o:thread/thread.c thread/thread.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/list.o:lib/kernel/list.c lib/kernel/list.h
	$(CC) $(CFLAGS) -o $@ $<

$(DIR)/kernel.o:kernel/kernel.S
	$(ASM) $(ASFLAGS) -o $@ $<
$(DIR)/print.o:lib/kernel/print.S
	$(ASM) $(ASFLAGS) -o $@ $<
$(DIR)/switch.o:thread/switch.S
	$(ASM) $(ASFLAGS) -o $@ $<

$(DIR)/kernel.bin:$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^



dir:
	 @if [ ! -d $(BUILD_DIR) ];then mkdir $(BUILD_DIR);fi 	

hd:
	@dd if=$(DIR)/kernel.bin of=$(BOCHS)/hd60M.img bs=512 count=200 seek=9 conv=notrunc

clean:
	cd $(DIR) && rm -f ./* && echo "remove ./build all is done!!"

build:$(DIR)/kernel.bin

all:dir build hd

.PHONY:dir hd clean all build


