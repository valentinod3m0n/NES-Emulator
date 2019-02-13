#include "nes.h"

int main(int argc, char *argv[]){
	if (argc < 2){
		printf("USO: nes_emulator <rom_path>\n");
		exit(EXIT_FAILURE);
	}

	char * rom_path = argv[1];

	NES nes;
	nes.rom = load_cartridge(rom_path);
	//nes.mapper = nes.mapper->create_mapper(nes.rom);
	
	free(nes.rom);
	printf("ROM WAS SUCCESFULLY READ\n");
	return 0;
}
