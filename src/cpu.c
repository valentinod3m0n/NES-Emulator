#include "cpu.h"

struct CPU{
	Address pc;
	uint8_t sp;
	uint8_t flags;

	uint8_t x;
	uint8_t y;
	uint8_t a;

	uint8_t irq;

	uint8_t * stack;
	uint8_t * ram;
}


uint8_t read_zero_page(CPU * cpu){
	read_ram(read_imm_8(cpu));
}

uint8_t read_zero_page_x(CPU * cpu){
	read_ram(read_imm_8(cpu) + cpu->x);
}

uint8_t read_zero_page_y(CPU * cpu){
	read_ram(read_imm_8(cpu) + cpu->y);
}

uint8_t read_absolute(CPU * cpu){
	read_ram(read_imm_16(cpu));
}
