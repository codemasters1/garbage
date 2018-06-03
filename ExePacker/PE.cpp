// PE.cpp by gynvael.coldwind//vx
// http://re.coldwind.pl
// http://gynvael.coldwind.pl
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <winnt.h>
#include <vector>
#include "PE.h"

using namespace std;

PESection::PESection()
{
  this->Data = NULL;
  memset(&this->Header, 0, sizeof(this->Header));
}

PESection::~PESection()
{
  if(this->Data)
    delete Data;
}

void PE::PadTo(FILE *f, size_t addr)
{
  static BYTE Zeros[256];
  size_t Offset = ftell(f);

  if(addr < Offset)
    return;

  size_t Diff = addr - Offset;

  while(Diff > 0)
  {
    if(Diff > sizeof(Zeros))
    {
      Diff -= sizeof(Zeros);
      fwrite(Zeros, 1, sizeof(Zeros), f);
    }
    else
    {
      fwrite(Zeros, 1, Diff, f);
      Diff = 0;
    }
  }
}

void PE::AlignTo(FILE *f, size_t align)
{
  size_t Offset = ftell(f);
  if(Offset % align == 0)
    return;

  this->PadTo(f, (Offset / align + 1) * align);
}


PE::PE()
{
  memset(&this->DosHeader, 0, sizeof(this->DosHeader));
  memset(&this->NtHeaders, 0, sizeof(this->NtHeaders));
}

PE::~PE()
{
  size_t i;
  for(i = 0; i < this->Sections.size(); i++)
    delete this->Sections[i];

  this->Sections.clear();
}

WORD PE::RVAtoSection(DWORD rva)
{
  size_t i;
  for(i = 0; i < this->Sections.size(); i++)
  {
    if(this->Sections[i]->Header.VirtualAddress <= rva &&
       this->Sections[i]->Header.SizeOfRawData + this->Sections[i]->Header.VirtualAddress > rva)
      return i;
  }

  return 0xffff;
}

BYTE *PE::RVAtoPtr(DWORD rva)
{
  WORD SecNo = this->RVAtoSection(rva);
  if(SecNo == 0xffff)
    return NULL;

  return &this->Sections[SecNo]->Data[rva - this->Sections[SecNo]->Header.VirtualAddress];
}

bool PE::Load(const char *name)
{
  FILE *f = fopen(name, "rb");
  if(!f)
    return false;

  fread(&this->DosHeader, 1, sizeof(this->DosHeader), f);
  fseek(f, this->DosHeader.e_lfanew, SEEK_SET);
  fread(&this->NtHeaders, 1, sizeof(this->NtHeaders), f);

  // sections
  int i;
  for(i = 0; i < this->NtHeaders.FileHeader.NumberOfSections; i++)
  {
    PESection *Section = new PESection;
    fread(&Section->Header, 1, sizeof(Section->Header), f);
    size_t Offset = ftell(f);
    fseek(f, Section->Header.PointerToRawData, SEEK_SET);
    Section->Data = new BYTE[Section->Header.SizeOfRawData];
    fread(Section->Data, 1, Section->Header.SizeOfRawData, f);
    fseek(f, Offset, SEEK_SET);
    this->Sections.push_back(Section);
  }

  fclose(f);
  return true;
}

bool PE::Dump(const char *name)
{
  FILE *f = fopen(name, "wb");
  if(!f)
    return false;

  fwrite(&this->DosHeader, 1, sizeof(this->DosHeader), f);
  this->PadTo(f, this->DosHeader.e_lfanew);
  fwrite(&this->NtHeaders, 1, sizeof(this->NtHeaders), f);
  
  size_t Offset = ftell(f);
  this->PadTo(f, Offset + sizeof(IMAGE_SECTION_HEADER) * this->NtHeaders.FileHeader.NumberOfSections);

  int i;
  for(i = 0; i < this->NtHeaders.FileHeader.NumberOfSections; i++)
  {
    this->AlignTo(f, this->NtHeaders.OptionalHeader.FileAlignment);
    this->Sections[i]->Header.PointerToRawData = ftell(f);
    fwrite(this->Sections[i]->Data, 1, this->Sections[i]->Header.SizeOfRawData, f);
  }

  fseek(f, Offset, SEEK_SET);
  for(i = 0; i < this->NtHeaders.FileHeader.NumberOfSections; i++)
    fwrite(&this->Sections[i]->Header, 1, sizeof(this->Sections[i]->Header), f);
  
  fclose(f);
  return true;
}