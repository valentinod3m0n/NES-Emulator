#include "cartridge.h"

struct Cartridge{
    uint8_t NES[4];
    uint8_t pgr_banks_count;
    uint8_t chr_banks_count;
    //ROM CONTROL BYTES:
    uint8_t rom_control_1;
    uint8_t rom_control_2;
    uint8_t ram_banks_count;
    uint8_t reserved[7];
};

struct NESROM{
    size_t pgr_size;
    size_t chr_size;
    uint8_t mapper;
    uint8_t battery;
    uint8_t mirroring;
    uint8_t *trainer;
    size_t ram_size;
    uint8_t *pgr_rom;
    uint8_t *chr_rom;
};

int check_nes_magic(uint8_t * nes_magic){
	//CONSIDERING IT'S LITTLE ENDIAN
	if (memcmp(nes_magic, "\x4e\x45\x53\x1a", 4 != 0)) return -1;
	else return 1;
}

void setup_from_rom_control(NESRom * rom, Cartridge cart){
	rom->pgr_size = cart.pgr_banks_count * PGR_BANK_SIZE;
	rom->chr_size = cart.chr_banks_count * CHR_BANK_SIZE;
	//rom_control_1 has the least significant bits
	rom->mapper = (cart.rom_control_1 >> 4) + (cart.rom_control_2 & 0xF0);
	rom->battery = cart.rom_control_1 & 0x2;
	rom->ram_size = cart.ram_banks_count * RAM_BANK_SIZE;
}

void set_screen_mirroring(NESRom * rom, Cartridge cart){
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
}

int has_trainer(Cartridge cart){
	if (cart.rom_control_1 & 0x4) return 1;
	else return 0;
}

int allocate_and_read_trainer(NESRom * rom, FILE *fp){
	rom->trainer = malloc(TRAINER_SIZE);
	if (fread(rom->trainer, TRAINER_SIZE, 1, fp) < 1){
		perror("fread");
		free(rom->trainer);
		return -1;
	}
	printf("The ROM has a trainer and it has been read.\n");
	return 1;
}


int allocate_and_read_pgr_rom(NESRom * rom, FILE *fp){
	if (rom->pgr_size == PGR_BANK_SIZE){
		//Needs to be mirrored if there's just 1 bank.
		rom->pgr_rom = malloc(rom->pgr_size);
		
		if (fread(rom->pgr_rom, rom->pgr_size, 1, fp) < 1){
			perror("fread");
			free(rom->pgr_rom);
			return -1;
		}
		
		printf("The ROM has a 1 PGR BANK and it has been read.\nPGR_ROM will be mirrored.\n");
		if (memcpy(&rom->pgr_rom[PGR_BANK_SIZE], rom->pgr_rom, PGR_BANK_SIZE) == NULL){
			perror("memcpy");
			free(rom->pgr_rom);
			return -1;
		}
	}else{
		rom->pgr_rom = malloc(rom->pgr_size);

		if (fread(rom->pgr_rom, rom->pgr_size, 1, fp) < 1){
			perror("fread");
			free(rom->pgr_rom);
			return -1;
		}
		printf("The ROM has more than 1 PGR BANK and they have been read.\n");
	}

	return 1;
}

int has_chr_rom(Cartridge cart){
	if (cart.chr_banks_count > 0) return 1;
	else return 0;
}

int allocate_and_read_chr_rom(NESRom * rom, FILE *fp){
	//Allocate memory for the CHR and read it.
	rom->chr_rom = malloc(rom->chr_size);

	if (fread(rom->chr_rom, rom->chr_size, 1, fp) < 1){
		perror("fread");
		free(rom->chr_rom);
		return -1;
	}
	printf("The ROM has CHR ROM and it has been read.\n");
	
	return 1;
}

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
	
	if (!check_nes_magic(cart.NES)){	
		printf("This is not a iNES file.\n");
		return NULL;
	}

	rom = malloc(sizeof(NESRom));
	setup_from_rom_control(rom, cart);
	
	if (rom->battery){
		printf("BATTERY DETECTED.\n");
	}
	
	set_screen_mirroring(rom, cart);
	
	//Allocate memory for the trainer and read it.
	if (has_trainer(cart)){
		if (allocate_and_read_trainer(rom, fp) == -1){
			free(rom);
			return NULL;
		}
	}

	if (allocate_and_read_pgr_rom(rom, fp) == -1){
		free(rom);
		return NULL;
	}
	
	
	if (has_chr_rom(cart)){
		if (allocate_and_read_chr_rom(rom, fp) == -1){
			free(rom);
			return NULL;
		}
	}

	fclose(fp);

	return rom;
}

void free_rom(NESRom * rom){
	printf("Freeing ROM.\n");
	free(rom->trainer);
	free(rom->pgr_rom);
	free(rom->chr_rom);
	free(rom);
}
