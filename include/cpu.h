#include <sys/types.h>

#define C_FLAG	0
#define Z_FLAG	1
#define I_FLAG	2
#define D_FLAG	3
#define B_FLAG	4
#define V_FLAG 	5

typedef struct CPU CPU;
typedef uint16_t Address;

static int set_flag(int reg, int flag){
	return reg |= (1 << flag);
}

static int clear_flag(int reg, int flag){
	return reg &= ~(1 << n);
}

static int test_flag(int reg, int flag){
	return reg & (1 << flag);
}
