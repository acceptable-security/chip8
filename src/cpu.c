#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"

chip8_op_t op_handlers[35] = {
	{ .opcode = 0x00E0, .opmask = 0xFFFF, .handler = chip8_op_disp_clear, .name = "CLS" },
	{ .opcode = 0x00EE, .opmask = 0xFFFF, .handler = chip8_op_ret, .name = "RET" },
	{ .opcode = 0x0000, .opmask = 0xF000, .handler = chip8_op_todo, .name = "SYS" },
	{ .opcode = 0x1000, .opmask = 0xF000, .handler = chip8_op_jump, .name = "JUMP" },
	{ .opcode = 0x2000, .opmask = 0xF000, .handler = chip8_op_call, .name = "CALL" },
	{ .opcode = 0x3000, .opmask = 0xF000, .handler = chip8_op_skip_equal_const, .name = "SKIP.EQ" },
	{ .opcode = 0x4000, .opmask = 0xF000, .handler = chip8_op_skip_not_equal_const, .name = "SKIP.NE" },
	{ .opcode = 0x5000, .opmask = 0xF00F, .handler = chip8_op_skip_equal, .name = "SKIP.EQ" },
	{ .opcode = 0x6000, .opmask = 0xF000, .handler = chip8_op_load_const, .name = "LD" },
	{ .opcode = 0x7000, .opmask = 0xF000, .handler = chip8_op_add_const, .name = "ADD" },
	{ .opcode = 0x8000, .opmask = 0xF00F, .handler = chip8_op_load_reg, .name = "LD" },
	{ .opcode = 0x8001, .opmask = 0xF00F, .handler = chip8_op_or, .name = "OR" },
	{ .opcode = 0x8002, .opmask = 0xF00F, .handler = chip8_op_and, .name = "AND" },
	{ .opcode = 0x8003, .opmask = 0xF00F, .handler = chip8_op_xor, .name = "XOR" },
	{ .opcode = 0x8004, .opmask = 0xF00F, .handler = chip8_op_add_overflow, .name = "ADD" },
	{ .opcode = 0x8005, .opmask = 0xF00F, .handler = chip8_op_sub_overflow, .name = "SUB" },
	{ .opcode = 0x8006, .opmask = 0xF00F, .handler = chip8_op_shr, .name = "SHR" },
	{ .opcode = 0x8007, .opmask = 0xF00F, .handler = chip8_op_subn_overflow, .name = "SUBN" },
	{ .opcode = 0x800E, .opmask = 0xF00F, .handler = chip8_op_shl, .name = "SHL" },
	{ .opcode = 0x9000, .opmask = 0xF00F, .handler = chip8_op_skip_not_equal, .name = "SNE" },
	{ .opcode = 0xA000, .opmask = 0xF000, .handler = chip8_op_write_ir, .name = "LD" },
	{ .opcode = 0xB000, .opmask = 0xF000, .handler = chip8_op_jump_ir, .name = "JP" },
	{ .opcode = 0xC000, .opmask = 0xF000, .handler = chip8_op_rand, .name = "RND" },
	{ .opcode = 0xD000, .opmask = 0xF000, .handler = chip8_op_disp_sprite, .name = "DRW" },
	{ .opcode = 0xE09E, .opmask = 0xF0FF, .handler = chip8_op_skip_keyboard, .name = "SKP" },
	{ .opcode = 0xE0A1, .opmask = 0xF0FF, .handler = chip8_op_skip_not_keyboard, .name = "SKNP" },
	{ .opcode = 0xF007, .opmask = 0xF0FF, .handler = chip8_op_get_delay_timer, .name = "LD" },
	{ .opcode = 0xF00A, .opmask = 0xF0FF, .handler = chip8_op_wait_keyboard, .name = "LD" },
	{ .opcode = 0xF015, .opmask = 0xF0FF, .handler = chip8_op_set_delay_timer, .name = "LD" },
	{ .opcode = 0xF018, .opmask = 0xF0FF, .handler = chip8_op_set_sound_timer, .name = "LD" },
	{ .opcode = 0xF01E, .opmask = 0xF0FF, .handler = chip8_op_add_ir, .name = "ADD" },
	{ .opcode = 0xF029, .opmask = 0xF0FF, .handler = chip8_op_get_font, .name = "LD" },
	{ .opcode = 0xF033, .opmask = 0xF0FF, .handler = chip8_op_load_bcd, .name = "LD" },
	{ .opcode = 0xF055, .opmask = 0xF0FF, .handler = chip8_op_load_lr, .name = "LD" },
	{ .opcode = 0xF065, .opmask = 0xF0FF, .handler = chip8_op_store_lr, .name = "LD" },
};

uint16_t chip8_read_short(chip8_state_t* state, uint16_t pos) {
	return (state->mem[pos] << 8) | state->mem[pos + 1];
}

void chip8_write_short(chip8_state_t* state, uint16_t pos, uint16_t data) {
	state->mem[pos] = (data >> 8) & 0xFF;
	state->mem[pos + 1] = data & 0xFF;
}

void chip8_next_op(chip8_state_t* state) {
	uint16_t op = chip8_read_short(state, state->ip);
	state->ip++;

	// Increment timers
	chip8_timer_tick(&state->dispt);
	chip8_timer_tick(&state->sndt);

	// Read the keyboard
	state->kb = state->kbread(false);

	// Dispatch the found opcode
	for ( int i = 0; i < sizeof(op_handlers) / sizeof(*op_handlers); i++ ) {
		if ( (op & op_handlers[i].opmask) == op_handlers[i].opcode ) {
			// printf("Found %s\n", op_handlers[i].name);
			chip8_status_t status = op_handlers[i].handler(state, op);

			switch ( status ) {
				case CHIP_ST_ERR:
					printf("ERROR\n");
					exit(1); // TODO: not this
				default:
					break;
			}
		}
	}
}
