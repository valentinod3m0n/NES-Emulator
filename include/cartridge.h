#ifndef CARTRIDGE_H_
#define CARTIRDGE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define HORIZONTAL_MIRRORING 	0
#define VERTICAL_MIRRORING		1
#define FOUR_SCREEN_MIRRORING	2
#define PGR_BANK_SIZE			0x4000
#define CHR_BANK_SIZE			0x2000
#define RAM_BANK_SIZE			0x2000
#define TRAINER_SIZE			0x200
#define NES_MAGIC_LE			"\x4e\x45\x53\x1a"

typedef struct Cartridge{
	uint8_t NES[4];
	uint8_t pgr_banks_count;
	uint8_t chr_banks_count;
	//ROM CONTROL BYTES:
	uint8_t rom_control_1;
	uint8_t rom_control_2;
	uint8_t ram_banks_count;
	uint8_t reserved[7];
} Cartridge;

typedef struct NESROM{
	size_t pgr_size;
	size_t chr_size;
	uint8_t mapper;
	uint8_t battery;
	uint8_t mirroring;
	uint8_t *trainer;
	size_t ram_size;
	uint8_t *pgr_rom;
	uint8_t *chr_rom;
} NESRom;

NESRom* load_cartridge(const char * path);
void free_rom(NESRom * rom);


#endif
