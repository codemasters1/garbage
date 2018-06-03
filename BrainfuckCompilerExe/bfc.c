#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "mylib.h"

int BFCompile(char *Code, size_t *CodeSize, char *Script, size_t ScriptSize);
DWORD AlignUp(DWORD Size, DWORD Alignment);
void falign(FILE *f, int Alignment);

int
main(int argc, char *argv[])
{
  char   *Script;
  char   *Code;
  size_t  ScriptSize;
  size_t  CodeSize;
  const char *OutName = "b.exe";
  static IMAGE_DOS_HEADER DosHeader;
  static IMAGE_NT_HEADERS NtHeaders;
  static IMAGE_SECTION_HEADER Section[2];
  static IMAGE_IMPORT_DESCRIPTOR Descriptor[2];
  FILE  *f;

  if(argc < 2)
  {
    puts("usage: bfc <ScriptName> [OutFile]");
    return 1;
  }

  if(argc == 3)
  {
    OutName = argv[2];
  }

  Script = readfile(argv[1], &ScriptSize);
  if(!Script)
  {
    puts("error: could not read script file");
    return 2;
  }

  CodeSize = ScriptSize * 20;
  Code = malloc(CodeSize);
  if(!Code)
  {
    puts("error: not enough memory");
    return 3;
  }

  if(BFCompile(Code, &CodeSize, Script, ScriptSize) != 0)
  {
    return 3;
  }  

  f = fopen(OutName, "wb");
  if(!f)
  {
    puts("error: could not create target file");
    return 3;
  }

  // DOS header
  DosHeader.e_magic = IMAGE_DOS_SIGNATURE;
  DosHeader.e_lfanew = 0x40;

  // NT headers
  NtHeaders.Signature = IMAGE_NT_SIGNATURE;
  NtHeaders.FileHeader.Machine          = 0x014C;
  NtHeaders.FileHeader.NumberOfSections = 2;
  NtHeaders.FileHeader.SizeOfOptionalHeader = sizeof(NtHeaders.OptionalHeader);
  NtHeaders.FileHeader.Characteristics  = 0x0307;

  // header 400000 - 400fff
  // .data  401000 - 408fff
  // .text  409000 - 40????
  //        409000 - imports
  //        409100 - kod

  NtHeaders.OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
  NtHeaders.OptionalHeader.AddressOfEntryPoint = 0x9100;
  NtHeaders.OptionalHeader.ImageBase = 0x00400000;
  NtHeaders.OptionalHeader.SectionAlignment = 0x1000;
  NtHeaders.OptionalHeader.FileAlignment = 0x200;
  NtHeaders.OptionalHeader.MajorOperatingSystemVersion = 4;
  NtHeaders.OptionalHeader.MinorOperatingSystemVersion = 0;
  NtHeaders.OptionalHeader.MajorSubsystemVersion = 4;
  NtHeaders.OptionalHeader.MinorSubsystemVersion = 0;
  NtHeaders.OptionalHeader.SizeOfImage = AlignUp(0x9100 + CodeSize, 0x1000);
  NtHeaders.OptionalHeader.SizeOfHeaders = 0x1000;
  NtHeaders.OptionalHeader.Subsystem = 3;
  NtHeaders.OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
  
  NtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = 0x9000;
  NtHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size           = 0x100;

  strcpy(Section[0].Name, ".data");
  Section[0].Misc.VirtualSize = 0x8000;
  Section[0].VirtualAddress = 0x1000;
  Section[0].SizeOfRawData = 0;
  Section[0].PointerToRawData = 0;
  Section[0].Characteristics = IMAGE_SCN_CNT_UNINITIALIZED_DATA |
                               IMAGE_SCN_MEM_READ |
                               IMAGE_SCN_MEM_WRITE;


  strcpy(Section[1].Name, ".text");
  Section[1].Misc.VirtualSize = AlignUp(0x100 + CodeSize, 0x1000);
  Section[1].VirtualAddress = 0x9000;
  Section[1].SizeOfRawData = 0x100 + CodeSize;
  Section[1].PointerToRawData = 
    AlignUp(sizeof(DosHeader) + sizeof(NtHeaders) + sizeof(Section), 0x200);
  Section[1].Characteristics = IMAGE_SCN_CNT_CODE |
                               IMAGE_SCN_CNT_INITIALIZED_DATA |
                               IMAGE_SCN_MEM_READ | 
                               IMAGE_SCN_MEM_EXECUTE;


  Descriptor[0].OriginalFirstThunk = 0x9080;
  Descriptor[0].Name               = 0x9028;
  Descriptor[0].FirstThunk         = 0x90A0;
  
  // 0x9000 Descriptor 0
  // 0x9014 Descriptor 1
  // 0x9028 Name "msvcrt.dll"
  // 0x9040 Import "putchar"
  // 0x9050 Import "getchar"
  // 0x9080 OriginalThunk[4] = { 0x9040, 0x9050, 0 ,0 }
  // 0x90A0 FirstThunk[4]    = { 0x9040, 0x9050, 0 ,0 }


  fwrite(&DosHeader, 1, sizeof(DosHeader), f);
  fwrite(&NtHeaders, 1, sizeof(NtHeaders), f);
  fwrite(Section, 1, sizeof(Section), f);
  falign(f, 0x200);

  fwrite(Descriptor, 1, sizeof(Descriptor), f);
  fwrite("msvcrt.dll", 1, 11, f);
  falign(f, 0x40);
  fwrite("\1\0putchar", 1, 10, f);
  falign(f, 0x10);
  fwrite("\1\0getchar", 1, 10, f);
  falign(f, 0x40);
  fwrite("\x40\x90\0\0" "\x50\x90\0\0", 1, 8, f);
  falign(f, 0x20);
  fwrite("\x40\x90\0\0" "\x50\x90\0\0", 1, 8, f);
  falign(f, 0x100);

  fwrite(Code, 1, CodeSize, f);

  fclose(f);
  free(Code);

  return 0;
}

DWORD AlignUp(DWORD Size, DWORD Alignment)
{
  if((Size % Alignment) != 0)
  {
    Size /= Alignment;
    Size++;
    Size *= Alignment;
  }

  return Size;
}

void falign(FILE *f, int Alignment)
{
  static ZeroBuffer[0x1000];
  size_t Pos, ToWrite = 0;
  fflush(f);
  Pos = ftell(f);


  if((Pos % Alignment) == 0)
    return;

  ToWrite = Alignment - (Pos % Alignment);

  while(ToWrite > 0)
  {
    if(ToWrite >= sizeof(ZeroBuffer))
    {
      fwrite(ZeroBuffer, 1, sizeof(ZeroBuffer), f);
      ToWrite -= sizeof(ZeroBuffer);
    }
    else
    {
      fwrite(ZeroBuffer, 1, ToWrite, f);
      ToWrite = 0;
    }
  }
}

char *writebuffer(char *Code, const void *What, size_t Write, size_t *Left)
{
  if(Write > *Left)
  {
    puts("error: design error");
    exit(1);
  }

  memcpy(Code, What, Write);
  *Left -= Write;

  return Code + Write;  
}

int
BFCompile(char *Code, size_t *CodeSize, char *Script, size_t ScriptSize)
{
  const unsigned char code_move_r[] = { 0x46 }; // inc esi
  const unsigned char code_move_l[] = { 0x4E }; // dec esi
  const unsigned char code_inc[] = { 0xFE, 0x06 }; // inc byte [esi]
  const unsigned char code_dec[] = { 0xFE, 0x0E }; // dec byte [esi]
  const unsigned char code_putchar[] = {
    0x31, 0xC0, // xor eax, eax
    0x8A, 0x06, // mov al, [esi]
    0x50,       // push eax
    0xFF, 0x15, 0xA0, 0x90, 0x40, 0x00, // call [IAT.putchar]
    0x81, 0xC4, 0x04, 0x00, 0x00, 0x00  // add esp, 4
  };
  const unsigned char code_getchar[] = {
    0xFF, 0x15, 0xA4, 0x90, 0x40, 0x00, //  call [IAT.getchar]
    0x88, 0x06                          //  mov [esi],al

  };
  const unsigned char code_epilogue[] = { 0xC3 }; // ret
  const unsigned char code_prologue[] = { 0xBE, 0x00, 0x10, 0x40, 0x00 }; // mov esi, .data
  const unsigned char code_jump_front[] = 
  {
   0x80, 0x3E, 0x00,             // cmp byte [esi],0x0
   0x75, 0x07,                   // jnz 0x31
   0xB8, 0x78, 0x56, 0x34, 0x12, // mov eax,0x12345678
   0xFF, 0xE0                    // jmp eax
  };
  const unsigned char code_jump_end[] =
  {
   0xB8, 0x78, 0x56, 0x34, 0x12, //  mov eax,0x12345678
   0xFF, 0xE0                    //  jmp eax
  };
  const int code_jump_start_offset = 6;
  const int code_jump_end_offset   = 1;
  size_t Left = *CodeSize; 
  size_t i;
  char *JumpStack[128], *Temp;
  int StackPointer = -1;
  unsigned int addr1, addr2;
  char *OrgCode = Code;

  //  N = Code - OrgCode

  // [ JumpStack[++StackPointer]
  // ] JumpStack[StackPointer--]

#define EMIT(a) Code = writebuffer(Code, a, sizeof(a), &Left)

  EMIT(code_prologue);

  for(i = 0; i < ScriptSize; i++)
  {
    switch(Script[i])
    {
      case '+': EMIT(code_inc); break;
      case '-': EMIT(code_dec); break;
      case '>': EMIT(code_move_r); break;
      case '<': EMIT(code_move_l); break;
      case '.': EMIT(code_putchar); break;
      case ',': EMIT(code_getchar); break;
      case '[': if(StackPointer + 1 >= sizeof(JumpStack) / sizeof(*JumpStack))
                {
                  puts("error: too many nest levels");
                  return 1;
                }
                JumpStack[++StackPointer] = Code;
                EMIT(code_jump_front);
                break;

      case ']': if(StackPointer < 0)
                {
                  puts("error: negative nested levels not allowed, missing opening [ ?");
                  return 2;
                }
                Temp = Code;
                EMIT(code_jump_end);

                addr1 = 0x409100 + (unsigned int)(Code - OrgCode);
                addr2 = 0x409100 + (unsigned int)(JumpStack[StackPointer] - OrgCode);

                *(unsigned int*)(JumpStack[StackPointer] + code_jump_start_offset) = addr1;
                *(unsigned int*)(Temp + code_jump_end_offset) = addr2;

                StackPointer--;
                break;
    }
  }
  
  EMIT(code_epilogue);

#undef EMIT




  *CodeSize -= Left;

  return 0;
}



