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
};

CPU * initialize(CPU * cpu){
	cpu->sp = 0;
	cpu->flags = 0x34;

	cpu->ram = malloc(sizeof(uint8_t) * RAM_SIZE);
	cpu->stack = &cpu->ram[415];

	cpu->x = 0;
	cpu->y = 0;
	cpu->a = 0;

	return cpu;
}

void free_cpu(CPU * cpu){
	free(cpu->ram);
	free(cpu->stack);
	free(cpu);
}

uint8_t read_memory(CPU * cpu, Address addr){
	switch(addr){
		case 0x0000 ... 0x07FF: return cpu->ram[addr];
		case 0x0800 ... 0x1FFF: return cpu->ram[addr % 0x800]; //Mirrors $0000-$07FF
		case 0x2008 ... 0x3FFF: return cpu->ram[addr % 0x2008 % 8 + 0x2000]; //Mirrors $2000-$2007
		//case 0x01A0 ... 0x01FF: {} //TODO: STACK, can this happen
	}
}

uint16_t read_imm_16(CPU * cpu){
	cpu->pc += 2;
	return read_memory(cpu, cpu->pc - 1) + (read_memory(cpu, cpu->pc) << 2);
}

uint8_t read_imm_8(CPU * cpu){
	return read_memory(cpu, ++cpu->pc);
}

uint8_t read_zero_page(CPU * cpu){
	read_memory(cpu, read_imm_8(cpu));
}

uint8_t read_zero_page_x(CPU * cpu){
	read_memory(cpu, read_imm_8(cpu) + cpu->x);
}

uint8_t read_zero_page_y(CPU * cpu){
	read_memory(cpu, read_imm_8(cpu) + cpu->y);
}

uint8_t read_absolute(CPU * cpu){
	read_memory(cpu, read_imm_16(cpu));
}

uint8_t read_absolute_x(CPU * cpu){
	read_memory(cpu, read_imm_16(cpu) + cpu->x);
}

uint8_t read_absolute_y(CPU * cpu){
	read_memory(cpu, read_imm_16(cpu) + cpu->y);
}

uint8_t read_indirect(CPU * cpu){
	uint16_t indirect_addr = read_imm_16(cpu);
	uint8_t direct_addr_low = read_memory(cpu, indirect_addr);
	uint8_t direct_addr_high = read_memory(cpu, indirect_addr + 1);

	return read_memory(cpu, direct_addr_low + direct_addr_high << 2);
}
