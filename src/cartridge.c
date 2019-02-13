#include "cartridge.h"

NESRom* load_cartridge(const char* path){
	Cartridge cart;
	NESRom* rom;
	FILE *fp;
	fp = fopen(path, "r");

	if (fp == NULL){
		perror("fopen");
		return NULL;
	}

	if (fread(&cart, sizeof(Cartridge), 1, fp) < 1){
		perror("fread");
		return NULL;
	}
	
	//CONSIDERING IT'S LITTLE ENDIAN
	if (memcmp(cart.NES, "\x4e\x45\x53\x1a", 4 != 0)){
		printf("This is not a iNES file.\n");
		return NULL;
	}

	rom = malloc(sizeof(NESRom));

	rom->pgr_size = cart.pgr_banks_count * PGR_BANK_SIZE;
	rom->chr_size = cart.chr_banks_count * CHR_BANK_SIZE;
	//rom_control_1 has the least significant bits
	rom->mapper = (cart.rom_control_1 >> 4) + (cart.rom_control_2 & 0xF0);
	rom->battery = cart.rom_control_1 & 0x2;
	rom->ram_size = cart.ram_banks_count * RAM_BANK_SIZE;
	
	if (rom->battery){
		printf("BATTERY DETECTED.\n");
	}

	if (cart.rom_control_1 & 0x8){
		rom->mirroring = FOUR_SCREEN_MIRRORING;
		printf("FOUR SCREEN MIRRORING DETECTED.\n");
	}else if (cart.rom_control_1 & 0x1){
		rom->mirroring = VERTICAL_MIRRORING;
		printf("VERTICAL MIRRORING DETECTED.\n");
	}else{
		rom->mirroring = HORIZONTAL_MIRRORING;
		printf("HORIZONTAL MIRRORING DETECTED.\n");
	}
	
	//Allocate memory for the trainer and read it.
	if (cart.rom_control_1 & 0x4){
		rom->trainer = malloc(TRAINER_SIZE);
		if (fread(rom->trainer, TRAINER_SIZE, 1, fp) < 1){
			perror("fread");
			free(rom);
			return NULL;
		}
		printf("The ROM has a trainer and it has been read.\n");
	}

	if (cart.pgr_banks_count == 1){
		//Needs to be mirrored if there's just 1 bank.
		rom->pgr_rom = malloc(rom->pgr_size);
		
		if (fread(rom->pgr_rom, rom->pgr_size, 1, fp) < 1){
			perror("fread");
			free(rom);
			return NULL;
		}
		printf("The ROM has a 1 PGR BANK and it has been read.\n");
		if (memcpy(&rom->pgr_rom[PGR_BANK_SIZE], rom->pgr_rom, PGR_BANK_SIZE) == NULL){
			perror("memcpy");
			free(rom);
			return NULL;
		}
	}else{
		rom->pgr_rom = malloc(rom->pgr_size);

		if (fread(rom->pgr_rom, rom->pgr_size, 1, fp) < 1){
			perror("fread");
			free(rom);
			return NULL;
		}
		printf("The ROM has more than 1 PGR BANK and they have been read.\n");
	}
	
	//We might have a ROM without CHR ROM.
	if (rom->chr_size > 0){
		//Allocate memory for the CHR and read it.
		rom->chr_rom = malloc(rom->chr_size);

		if (fread(rom->chr_rom, rom->chr_size, 1, fp) < 1){
			perror("fread");
			free(rom);
			return NULL;
		}
		printf("The ROM has CHR ROM and it has been read.\n");
	}

	fclose(fp);

	return rom;
}

void free_rom(NESRom * rom){
	free(rom->trainer);
	free(rom->pgr_rom);
	free(rom->chr_rom);
	free(rom);
}
