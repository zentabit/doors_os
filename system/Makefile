C_SOURCES = $(wildcard kernel/*.c drivers/*.c interrupt/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h interrupt/*.h)

OBJ = ${C_SOURCES:.c=.o interrupt/int.o}

all: os.img

run: all
	bochs

os.img: boot/bootcode.bin kernel/kernel.bin
	cat $^ > os.img

kernel/kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

%.o : %.c ${HEADERS}
	gcc -ffreestanding -m32 -fno-pie -c $< -o $@

%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm $< -f bin -i 'boot/' -o $@

clean:
	rm -rf *.bin *.dis *.o os.img kernel/*.bin
	rm -rf kernel/*.o boot/*.bin drivers/*.o interrupt/*.o