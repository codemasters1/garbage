#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define SECTION_LOADER_NAME "K13Crypt"

typedef struct 
{
	IMAGE_SECTION_HEADER Header;
	BYTE *Data;
} Section_s;

void fpadto(FILE *f, size_t addr)
{
    static BYTE zeros[256];
    size_t offset = ftell(f);

    if(addr < offset)
        return;

    size_t diff = addr - offset;

    while(diff > 0)
    {
        if(diff > sizeof(zeros))
        {
			diff -= sizeof(zeros);
			fwrite(zeros, 1, sizeof(zeros), f);
        }
        else
        {
			fwrite(zeros, 1, diff, f);
			diff = 0;
        }
    }
}

void falignto(FILE *f, size_t align)
{
    size_t offset = ftell(f);
    if(offset % align == 0)
        return;

    fpadto(f, (offset / align + 1) * align);
}

DWORD RvaToPtr(Section_s *Sections[], size_t n, DWORD rva)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (rva >= Sections[i]->Header.VirtualAddress && rva < Sections[i]->Header.VirtualAddress + Sections[i]->Header.Misc.VirtualSize)
		{
			return Sections[i]->Header.PointerToRawData + rva - Sections[i]->Header.VirtualAddress;
		}
	}
	
	return 0;
}

size_t LoadLoader(char *file, BYTE *buff)
{
	FILE *f = fopen(file, "rb");
	buff = malloc(0x1000);
	memset(buff, 0, 0x1000);
	size_t size;
	size = fread(buff, 1, 0x1000, f);
	fclose(f);
	
}


int main(int argc, char *argv[])
{
	puts("Encrypter by K13 (c) 2013\n");

    if (argc < 3)
    {
        printf("usage: %s <input.exe> <output.exe>", argv[0]);
        return 1;
    }

    IMAGE_DOS_HEADER DosHeader;
    IMAGE_NT_HEADERS NtHeaders;

    memset(&DosHeader, 0, sizeof(DosHeader));
    memset(&NtHeaders, 0, sizeof(NtHeaders));


    FILE *file_input = fopen(argv[1], "rb");
	
	if (file_input == NULL)
	{
		puts("error: could not open input file");
		return 2;
	}
	
	fseek(file_input, 0, SEEK_END);
	size_t file_size = ftell(file_input);
	fseek(file_input, 0, SEEK_SET);
	
	char *File = malloc(file_size);
	fread(File, 1, file_size, file_input);
	fclose(file_input);
	
	size_t Pos = 0;
	memcpy(&DosHeader, &File[Pos], sizeof(IMAGE_DOS_HEADER));
	Pos += DosHeader.e_lfanew;
	memcpy(&NtHeaders, &File[Pos], sizeof(IMAGE_NT_HEADERS));
	Pos += sizeof(IMAGE_NT_HEADERS);
    //fread(&DosHeader, 1, sizeof(DosHeader), file_input);
    //fseek(file_input, DosHeader.e_lfanew, SEEK_SET);
    //fread(&NtHeaders, 1, sizeof(NtHeaders), file_input);
	
	//printf("%4s", (char *) &NtHeaders.Signature);
	
	//if (memcmp(&NtHeaders.Signature, "PE\0\0", 4) != 0)
	//{
	//	puts("error: invalid PE file");
	//	return 3;
	//}
	
	
	if (NtHeaders.FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
	{
		puts("error: crypter supports only x86 executables");
		return 3;
	}


	Section_s *Sections[NtHeaders.FileHeader.NumberOfSections + 1];

	printf("Found %d section(s).\n", NtHeaders.FileHeader.NumberOfSections);
	
	int i;
    for (i = 0; i < NtHeaders.FileHeader.NumberOfSections; i++)
    {
		Sections[i] = malloc(sizeof(Section_s));
		memset(Sections[i], 0, sizeof(IMAGE_SECTION_HEADER));
		memcpy(&Sections[i]->Header, &File[Pos], sizeof(IMAGE_SECTION_HEADER));
		
		//fread(&Sections[i]->Header, 1, sizeof(IMAGE_SECTION_HEADER), file_input);
		size_t offset = Pos + sizeof(IMAGE_SECTION_HEADER);
		
		Sections[i]->Data = malloc(Sections[i]->Header.SizeOfRawData);
		memset(Sections[i]->Data, 0, Sections[i]->Header.SizeOfRawData);
		memcpy(Sections[i]->Data, File + Sections[i]->Header.PointerToRawData, Sections[i]->Header.SizeOfRawData);
		//fseek(file_input, Sections[i]->Header.PointerToRawData, SEEK_SET);
		//fread(Sections[i]->Data, 1, Sections[i]->Header.SizeOfRawData, file_input);
		
		
        Pos = offset;
		
		//fseek(file_input, offset, SEEK_SET);
		
		if (strcmp(Sections[i]->Header.Name, SECTION_LOADER_NAME) == 0)
		{
			puts("error: file is already encrypted");
			
			return 4;
		}
		
		printf("%s: 0x%p - 0x%p\n", 
			Sections[i]->Header.Name,
			Sections[i]->Header.PointerToRawData,
			Sections[i]->Header.PointerToRawData + Sections[i]->Header.SizeOfRawData - 1);
		
    }
	
	//printf("%i", NtHeaders.OptionalHeader.NumberOfRvaAndSizes);
	
	printf("IAT = %p - %p\n", 
		NtHeaders.OptionalHeader.DataDirectory[1].VirtualAddress,
		NtHeaders.OptionalHeader.DataDirectory[1].VirtualAddress + NtHeaders.OptionalHeader.DataDirectory[1].Size);
		
	// iat to file
	
	DWORD ImportPos = RvaToPtr(
		Sections,
		NtHeaders.FileHeader.NumberOfSections,
		NtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
	);
	
	if (ImportPos == 0)
	{
		puts("error: could not get Import Table Directory offset");
		return 5;
	}
	
	//IMAGE_IMPORT_DESCRIPTOR tmp;
	//memset(&tmp, 0, sizeof(tmp));
	
	//for(;;)
	//{
	//	memcpy(&tmp, File + ImportPos, sizeof(tmp));
		
	//	if (tmp.FirstThunk == 0 && tmp.OriginalFirstThunk == 0)
	//		break;

		
		
	//	ImportPos += sizeof(tmp);
		
	//}
	
	//IMAGE_IMPORT_DESCRIPTOR Import
	
	//FILE *f = fopen("itd.bin", "wb");
	//fwrite(&File[ImportPos], 1, NtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size, f);
	//fclose(f);
	
	//free(File);
	
	// new section for loader
	Section_s LoaderSec;
	memset(&LoaderSec, 0, sizeof(LoaderSec));
	
	memcpy(LoaderSec.Header.Name, SECTION_LOADER_NAME, 8);
	LoaderSec.Header.Misc.VirtualSize = 0x1000;
	LoaderSec.Header.VirtualAddress = ((NtHeaders.OptionalHeader.SizeOfImage - 1) / 0x1000 + 1) * 0x1000;
	LoaderSec.Header.Characteristics = 
		  IMAGE_SCN_MEM_EXECUTE
		| IMAGE_SCN_MEM_READ 		
		| IMAGE_SCN_MEM_WRITE 
		| IMAGE_SCN_CNT_CODE 
		| IMAGE_SCN_CNT_INITIALIZED_DATA;
	
	DWORD OEP = NtHeaders.OptionalHeader.AddressOfEntryPoint + NtHeaders.OptionalHeader.ImageBase;
	DWORD SectionStart = Sections[0]->Header.VirtualAddress + NtHeaders.OptionalHeader.ImageBase;
	
	
	//printf("\nOEP = 0x%p\n", OEP);
	

	BYTE *Loader;
	LoaderSec.Header.SizeOfRawData = LoadLoader("loader", Loader);
	LoaderSec.Data = Loader;
	
	memcpy(&Loader[0], &OEP, 4);
	memcpy(&Loader[4], &SectionStart, 4);
	memcpy(&Loader[8], &Sections[0]->Header.SizeOfRawData, 4);
	
	
	// changing PE entry point
	NtHeaders.OptionalHeader.AddressOfEntryPoint = LoaderSec.Header.VirtualAddress + 0x200;

	Sections[NtHeaders.FileHeader.NumberOfSections] = &LoaderSec;
	NtHeaders.FileHeader.NumberOfSections++;
	
	NtHeaders.OptionalHeader.SizeOfImage = LoaderSec.Header.VirtualAddress + LoaderSec.Header.Misc.VirtualSize;
	
	
	// encrypting code section
	for(i = 0; i < Sections[0]->Header.SizeOfRawData; i++)
	{
		Sections[0]->Data[i] += (unsigned char)(i+1);
		Sections[0]->Data[i] ^= 0x89;
	}

	Sections[0]->Header.Characteristics |= IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ;
	
	
	FILE *file_output = fopen(argv[2], "wb");
	
	if (file_output == NULL)
	{
		puts("error: could not open output file");
		
		return 6;
	}
	
	fwrite(&DosHeader, 1, sizeof(DosHeader), file_output);
	fpadto(file_output, DosHeader.e_lfanew);
	fwrite(&NtHeaders, 1, sizeof(NtHeaders), file_output);
	
	size_t Offset = ftell(file_output);
	fpadto(file_output, Offset + sizeof(IMAGE_SECTION_HEADER) * NtHeaders.FileHeader.NumberOfSections);
	
	
	for(i = 0; i < NtHeaders.FileHeader.NumberOfSections; i++)
	{
		falignto(file_output, NtHeaders.OptionalHeader.FileAlignment);
		Sections[i]->Header.PointerToRawData = ftell(file_output);
		fwrite(Sections[i]->Data, 1, Sections[i]->Header.SizeOfRawData, file_output);
	}
	
	fseek(file_output, Offset, SEEK_SET);

	for (i = 0; i < NtHeaders.FileHeader.NumberOfSections; i++)
	{
		fwrite(&Sections[i]->Header, 1, sizeof(IMAGE_SECTION_HEADER), file_output);

		free(Sections[i]->Data);
		free(Sections[i]);
	}

    fclose(file_output);
	
	puts("END");

    return 0;
}
