// PE.h by gynvael.coldwind//vx
// http://re.coldwind.pl
// http://gynvael.coldwind.pl
#pragma once
#include <windows.h>
#include <winnt.h>
#include <vector>
using namespace std;

class PESection
{
  public:
    IMAGE_SECTION_HEADER Header;
    BYTE *Data;

    PESection();
    ~PESection();
};

class PE
{
  private:
    void PadTo(FILE *f, size_t addr);
    void AlignTo(FILE *f, size_t addr);
    

  public:
    IMAGE_DOS_HEADER DosHeader;
    IMAGE_NT_HEADERS NtHeaders;
    vector<PESection *> Sections;

    PE();
    ~PE();

    bool Load(const char *name);
    bool Dump(const char *name);
	
	BYTE *RVAtoPtr(DWORD rva);
	WORD RVAtoSection(DWORD rva);
};

