#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "binder.h"

int main(int argc, char *argv[])
{
	TCHAR fname[MAX_PATH];
	IMAGE_DOS_HEADER DosHeader;
	IMAGE_NT_HEADERS NtHeaders;
	HANDLE hFile;
	DWORD bytes = 0;
	BINDER_HEADER header;

	GetModuleFileName(NULL, fname, MAX_PATH);
	hFile = CreateFile(
		fname,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		puts("error: could not open file");
		return 1;
	}
	
	ReadFile(hFile, &DosHeader, sizeof(IMAGE_DOS_HEADER), &bytes, NULL);
	SetFilePointer(hFile, DosHeader.e_lfanew, NULL, FILE_BEGIN);
	ReadFile(hFile, &NtHeaders, sizeof(IMAGE_NT_HEADERS), &bytes, NULL);
	SetFilePointer(
		hFile, 
		(NtHeaders.FileHeader.NumberOfSections - 1) * sizeof(IMAGE_SECTION_HEADER), 
		NULL,
		FILE_CURRENT);
	
	IMAGE_SECTION_HEADER lastSection;
	ReadFile(hFile, &lastSection, sizeof(IMAGE_SECTION_HEADER), &bytes, NULL);
	SetFilePointer(hFile, lastSection.PointerToRawData + lastSection.SizeOfRawData, NULL, FILE_BEGIN);
	
	ReadFile(hFile, &header, sizeof(BINDER_HEADER), &bytes, NULL);
	
	if (bytes == 0)
	{
		MessageBox(NULL, "Could not find expected header.", "Error.", 16);
		return 1;
	}
	
	printf("%p", header.Message.Options);
	
	MessageBoxW(NULL, header.Message.Message, header.Message.Title, header.Message.Options);
	
	CloseHandle(hFile);
	
	return 0;
}