all: chip8

.PHONY: chip8

chip8: src/cpu.c src/fb.c src/cpu_op.c src/timer.c
	gcc -o chip8 -Isrc/ src/cpu.c src/fb.c src/cpu_op.c src/timer.c

clean:
	rm -f runas

