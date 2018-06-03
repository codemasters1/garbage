#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void fpadto(FILE *f, size_t addr)
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

void falignto(FILE *f, size_t align)
{
    size_t Offset = ftell(f);
    if(Offset % align == 0)
        return;

    fpadto(f, (Offset / align + 1) * align);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("usage: kc <filename.exe>");
        return 1;
    }

    IMAGE_DOS_HEADER DosHeader;
    IMAGE_NT_HEADERS NtHeaders;

    memset(&DosHeader, 0, sizeof(DosHeader));
    memset(&NtHeaders, 0, sizeof(NtHeaders));

    FILE *input = fopen(argv[1], "rb");

    fread(&DosHeader, 1, sizeof(DosHeader), input);
    fseek(input, DosHeader.e_lfanew, SEEK_SET);
    fread(&NtHeaders, 1, sizeof(NtHeaders), input);

    int i;
    IMAGE_SECTION_HEADER tmp;
    for (i = 0; i < NtHeaders.FileHeader.NumberOfSections; i++)
    {
        fread(&tmp, 1, sizeof(tmp), input);
        size_t offset = ftell(input);
        printf("%s\n", tmp.Name);
        fseek(input, offset, SEEK_SET);
    }

    printf("%1.8x", NtHeaders.FileHeader.NumberOfSections);

    fclose(input);

    return 0;
}
