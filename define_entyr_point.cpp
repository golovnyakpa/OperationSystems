#include <iostream>
#include <elf.h>
#include <stdio.h>
#include <string.h>

#define ELF_NIDENT	16


struct elf_hdr {
	std::uint8_t e_ident[ELF_NIDENT];
	std::uint16_t e_type;
	std::uint16_t e_machine;
	std::uint32_t e_version;
	std::uint64_t e_entry;
	std::uint64_t e_phoff;
	std::uint64_t e_shoff;
	std::uint32_t e_flags;
	std::uint16_t e_ehsize;
	std::uint16_t e_phentsize;
	std::uint16_t e_phnum;
	std::uint16_t e_shentsize;
	std::uint16_t e_shnum;
	std::uint16_t e_shstrndx;
} __attribute__((packed));


std::uintptr_t entry_point(const char *name)
{
	// Ваш код здесь, name - имя ELF файла.
	struct elf_hdr header;
	FILE* file = fopen(name, "rb");
	if(file) {
		fread(&header, 1, sizeof(header), file);
		fclose(file);
	}
	return header.e_entry;
}

int main() 
{ 
    const char name[] = "test";
    entry_point(name);
    return 0; 
}
