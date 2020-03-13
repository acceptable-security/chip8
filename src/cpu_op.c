#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

chip8_status_t chip8_op_todo(chip8_state_t* state, uint16_t opcode) {
	printf("UNIMPLEMENTED %02x\n", opcode);
	return CHIP_ST_ERR;
}

chip8_status_t chip8_op_disp_clear(chip8_state_t* state, uint16_t opcode) {
	chip8_fb_clear(state->fb);
	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_ret(chip8_state_t* state, uint16_t opcode) {
	uint16_t nip = chip8_read_short(state, state->sp);
	state->sp--;
	state->ip = nip;
	return CHIP_ST_SET;
}

chip8_status_t chip8_op_jump(chip8_state_t* state, uint16_t opcode) {
	state->ip = opcode & 0xFFF;
	return CHIP_ST_SET;
}

chip8_status_t chip8_op_call(chip8_state_t* state, uint16_t opcode) {
	chip8_write_short(state, state->sp++, state->ip);
	state->ip = opcode & 0xFFF;
	return CHIP_ST_SET;
}

chip8_status_t chip8_op_skip_equal_const(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = opcode & 0xFF;

	if ( val1 == val2 ) {
		state->ip++;
		return CHIP_ST_SET;
	}

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_skip_not_equal_const(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = opcode & 0xFF;

	if ( val1 != val2 ) {
		state->ip++;
		return CHIP_ST_SET;
	}

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_skip_equal(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];

	if ( val1 == val2 ) {
		state->ip++;
		return CHIP_ST_SET;
	}

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_load_const(chip8_state_t* state, uint16_t opcode) {
	state->registers[(opcode >> 8) & 0xFF] = opcode & 0xFF;
	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_add_const(chip8_state_t* state, uint16_t opcode) {
	uint8_t reg_old = state->registers[(opcode >> 8) & 0xFF];
	reg_old += opcode & 0xFF;
	state->registers[(opcode >> 8) & 0xFF] = reg_old;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_load_reg(chip8_state_t* state, uint16_t opcode) {
	state->registers[(opcode >> 8) & 0xF] = state->registers[(opcode >> 4) & 0xF];
	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_or(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];

	state->registers[(opcode >> 8) & 0xF] = val1 | val2;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_and(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];

	state->registers[(opcode >> 8) & 0xF] = val1 & val2;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_xor(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];

	state->registers[(opcode >> 8) & 0xF] = val1 ^ val2;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_add_overflow(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];
	uint8_t val3 = val1 + val2;
	uint8_t of = val3 < val1 || val3 < val2;

	state->registers[(opcode >> 8) & 0xF] = val3;
	state->registers[CHIP8_FLAG_OF] = of;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_sub_overflow(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];
	uint8_t val3 = val1 - val2;
	uint8_t of = val1 > val2;

	state->registers[(opcode >> 8) & 0xF] = val3;
	state->registers[CHIP8_FLAG_OF] = of;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_shr(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	// uint8_t val2 = state->registers[(opcode >> 4) & 0xF];
	uint8_t val3 = val1 >> 1;
	uint8_t of = val1 & 1;

	state->registers[(opcode >> 8) & 0xF] = val3;
	state->registers[CHIP8_FLAG_OF] = of;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_subn_overflow(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];
	uint8_t val3 = val2 - val1;
	uint8_t of = val2 > val1;

	state->registers[(opcode >> 8) & 0xF] = val3;
	state->registers[CHIP8_FLAG_OF] = of;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_shl(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	// uint8_t val2 = state->registers[(opcode >> 4) & 0xF];
	uint8_t val3 = val1 << 1;
	uint8_t of = (val1 >> 7) & 1;

	state->registers[(opcode >> 8) & 0xF] = val3;
	state->registers[CHIP8_FLAG_OF] = of;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_skip_not_equal(chip8_state_t* state, uint16_t opcode) {
	uint8_t val1 = state->registers[(opcode >> 8) & 0xF];
	uint8_t val2 = state->registers[(opcode >> 4) & 0xF];

	if ( val1 != val2 ) {
		state->ip++;
		return CHIP_ST_SET;
	}

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_write_ir(chip8_state_t* state, uint16_t opcode) {
	uint16_t val1 = opcode & 0xFFF;
	state->ir = val1;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_jump_ir(chip8_state_t* state, uint16_t opcode) {
	uint16_t val1 = opcode & 0xFFF;
	uint16_t val2 = val1 + state->registers[0];
	
	state->ir = val2;

	return CHIP_ST_NXT;
}

chip8_status_t chip8_op_rand(chip8_state_t* state, uint16_t opcode) {
	uint8_t mask = opcode & 0xFF;
	uint8_t val3 = rand() & mask;

	state->registers[(opcode >> 8) & 0xF] = val3;

	return CHIP_ST_NXT;
}
