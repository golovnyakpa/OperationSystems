#include <iostream>

#define ELF_NIDENT	16

// program header-ы такого типа нужно загрузить в
// память при загрузке приложения
#define PT_LOAD		1

// структура заголовка ELF файла
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

// структура записи в таблице program header-ов
struct elf_phdr {
	std::uint32_t p_type;
	std::uint32_t p_flags;
	std::uint64_t p_offset;
	std::uint64_t p_vaddr;
	std::uint64_t p_paddr;
	std::uint64_t p_filesz;
	std::uint64_t p_memsz;
	std::uint64_t p_align;
} __attribute__((packed));


void fill_header_structure(const char *name, struct elf_hdr* header)
{
	FILE* file = fopen(name, "rb");
	if(file) {
		fread(header, 1, sizeof(*header), file);
		fclose(file);
	}
}


std::size_t space(const char *name)
{
    // Ваш код здесь, name - имя ELF файла, с которым вы работаете
    // вернуть нужно количество байт, необходимых, чтобы загрузить
    // приложение в память
    struct elf_hdr header;
    fill_header_structure(name, &header);
    
    struct elf_phdr pheader;
    std::size_t res = 0;
    
    FILE* file = fopen(name, "rb");
	if(file) {
		fseek(file, header.e_phoff, SEEK_SET);
		//read(header, 1, sizeof(*header), file);
		for (std::uint16_t i=0; i<header.e_phnum; ++i){
			fread(&pheader, 1, header.e_phentsize, file);
			if (pheader.p_type == PT_LOAD)
				res += pheader.p_memsz;
		}
		fclose(file);
	}
	//printf("%d\n", res);
	return res;
}

int main() 
{ 
    const char name[] = "test";
    space(name);
    //printf("%d", space(name));
    return 0; 
}
