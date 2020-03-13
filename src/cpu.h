#include <stdint.h>

#include "fb.h"

#define CHIP8_MEM_SIZE 0x1000
#define CHIP8_REG_SIZE 16
#define CHIP8_FLAG_OF  0xF

typedef struct {
	uint8_t memory[CHIP8_MEM_SIZE];    // Memory
	uint8_t registers[CHIP8_REG_SIZE]; // Registers
	uint16_t ir;                       // The I register
	uint16_t ip;                       // Instruction Pointer
	uint8_t  sp;                       // Stack pointer
	uint8_t display_timer;             // Display timer
	uint8_t sound_timer;               // Sound timer  
	chip8_fb_t* fb;                     // Framebuffer
} chip8_state_t;

typedef enum {
	// TODO: are the bottom two necessary?
	CHIP_ST_SET, // IP set from instruction
	CHIP_ST_NXT, // Go to next instruction
	CHIP_ST_ERR  // An error has occurred  
} chip8_status_t;

typedef chip8_status_t (*chip8_op_handler_t)(chip8_state_t*, uint16_t);

typedef struct {
	// (instr & opmask) == opcode
	uint16_t opcode;
	uint16_t opmask;
	chip8_op_handler_t handler;
	const char* name;
} chip8_op_t;

uint16_t chip8_read_short(chip8_state_t* state, uint16_t pos);
void chip8_write_short(chip8_state_t* state, uint16_t pos, uint16_t data);
void chip8_next_op(chip8_state_t* state);

chip8_status_t chip8_op_todo(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_disp_clear(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_ret(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_jump(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_jump(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_call(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_skip_equal_const(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_skip_not_equal_const(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_skip_equal(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_load_const(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_add_const(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_load_reg(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_or(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_and(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_xor(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_add_overflow(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_sub_overflow(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_shr(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_subn_overflow(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_shl(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_skip_not_equal(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_write_ir(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_jump_ir(chip8_state_t* state, uint16_t opcode);
chip8_status_t chip8_op_rand(chip8_state_t* state, uint16_t opcode);