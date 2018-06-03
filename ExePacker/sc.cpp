#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "PE.h"

using namespace std;

int
main(int argc, char *argv[])
{
  puts("SimpleCrypt");

  if(argc != 2)
  {
    puts("usage: sc <filename.exe>");
    return 1;
  }

  PE file;

  if(!file.Load(argv[1]))
  {
    puts("file not found or not an exe file");
    return 2;
  }

  // Dodajemy loader
  PESection *LoaderSec = new PESection;
  file.Sections.push_back(LoaderSec);
  file.NtHeaders.FileHeader.NumberOfSections++;

  memcpy(LoaderSec->Header.Name, "SimpCryp", 8);
  LoaderSec->Header.Misc.VirtualSize = 0x1000;
  LoaderSec->Header.VirtualAddress =
    ((file.NtHeaders.OptionalHeader.SizeOfImage - 1) / 0x1000 + 1)
    * 0x1000;
  LoaderSec->Header.Characteristics = 
    IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ |
    IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_CODE |
    IMAGE_SCN_CNT_INITIALIZED_DATA;

  FILE *f = fopen("loader", "rb");
  BYTE *Loader = new BYTE[0x1000];
  memset(Loader, 0, 0x1000);
  size_t ret;
  ret = fread(Loader, 1, 0x1000, f);
  fclose(f);

  LoaderSec->Header.SizeOfRawData = ret;
  LoaderSec->Data = Loader;

  file.NtHeaders.OptionalHeader.SizeOfImage = 
    LoaderSec->Header.VirtualAddress  +
    LoaderSec->Header.Misc.VirtualSize;

  DWORD OEP = file.NtHeaders.OptionalHeader.AddressOfEntryPoint
             +file.NtHeaders.OptionalHeader.ImageBase;

  memcpy(&Loader[0], &OEP, 4);
  
  int SectionImportIndex = 0;
  size_t ImportsRVA = file.NtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
  printf("Imports table RVA: %p\n", ImportsRVA);
  
  BYTE *ImportData = file.RVAtoPtr(ImportsRVA);
  
  int DescriptorIndex = 0;
  for(;;)
  {
	IMAGE_IMPORT_DESCRIPTOR desc;
	memcpy(&desc, ImportData + sizeof(desc) * DescriptorIndex++, sizeof(desc));
	
	if (desc.Name == 0)
	{
		break;
	}
	
	DWORD FirstThunk = desc.OriginalFirstThunk ? desc.OriginalFirstThunk : desc.FirstThunk;
	
	printf("firtthunk: %p\n", FirstThunk);
	
	int ThunkIndex = 0;
	if (FirstThunk != 0)
	{
		printf("%p\n", file.RVAtoPtr(FirstThunk));
	
		IMAGE_THUNK_DATA thunk;
		memcpy(&thunk, file.RVAtoPtr(FirstThunk) + sizeof(thunk) * ThunkIndex++, sizeof(thunk));
		
		printf("thunk: %p %p %p %p\n", thunk.u1.ForwarderString, thunk.u1.Function, thunk.u1.Ordinal, thunk.u1.AddressOfData);
		
		IMAGE_IMPORT_BY_NAME ibn;
		memcpy(&ibn, file.RVAtoPtr(thunk.u1.AddressOfData), sizeof(ibn));
		
		printf("hint - %p %p\n", ibn.Hint, file.RVAtoPtr(ibn.Hint));
	}
	
	printf("%s\n", file.RVAtoPtr(desc.Name));
  }
  
  //printf("Imports ptr: %p\n", ImportSection->Header.PointerToRawData);
  
 

  DWORD EncryptedSectionStart = file.Sections[SectionImportIndex]->Header.VirtualAddress
             +file.NtHeaders.OptionalHeader.ImageBase;
  memcpy(&Loader[4], &EncryptedSectionStart, 4);
  memcpy(&Loader[8], &file.Sections[SectionImportIndex]->Header.SizeOfRawData, 4);

  file.NtHeaders.OptionalHeader.AddressOfEntryPoint =
    LoaderSec->Header.VirtualAddress + 0x200;

  size_t i;
  for(i = 0; i < file.Sections[SectionImportIndex]->Header.SizeOfRawData; i++)
  {
    file.Sections[SectionImportIndex]->Data[i] += (unsigned char)(i + 1);
    file.Sections[SectionImportIndex]->Data[i] ^= 0x55;
  }

  file.Sections[SectionImportIndex]->Header.Characteristics |=
    IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ;

  file.Dump("sth.exe");

  return 0;
}