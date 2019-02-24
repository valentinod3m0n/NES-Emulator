#ifndef CPU_H_
#define CPU_H_

#include <stdlib.h>
#include <stdint.h>

#define C_FLAG	0
#define Z_FLAG	1
#define I_FLAG	2
#define D_FLAG	3
#define B_FLAG	4
#define V_FLAG 	6
#define N_FLAG	7

#define STACK_SIZE 96
#define RAM_SIZE 2048

typedef struct CPU CPU;
typedef uint16_t Address;

static int set_flag(int reg, int flag){
	return reg |= (1 << flag);
}

static int clear_flag(int reg, int flag){
	return reg &= ~(1 << flag);
}

static int test_flag(int reg, int flag){
	return reg & (1 << flag);
}


#endif