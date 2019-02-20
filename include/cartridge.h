#ifndef CARTRIDGE_H_
#define CARTIRDGE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define HORIZONTAL_MIRRORING 		0
#define VERTICAL_MIRRORING		1
#define FOUR_SCREEN_MIRRORING		2
#define PGR_BANK_SIZE			0x4000
#define CHR_BANK_SIZE			0x2000
#define RAM_BANK_SIZE			0x2000
#define TRAINER_SIZE			0x200
#define NES_MAGIC_LE			"\x4e\x45\x53\x1a"

typedef struct NESROM NESRom;
typedef struct Cartridge Cartridge;

NESRom* load_cartridge(const char * path);
void free_rom(NESRom * rom);


#endif
