all: chip8

.PHONY: chip8

chip8: src/cpu.c src/fb.c src/cpu_op.c
	gcc -o chip8 -Isrc/ src/cpu_op.c src/cpu.c src/fb.c

clean:
	rm -f runas

