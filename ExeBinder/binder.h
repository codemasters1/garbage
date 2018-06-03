#include <windows.h>

typedef struct {
	BYTE Reserved[128];
} BINDER_OPTIONS;

typedef struct {
	WCHAR Title[64];			
	WCHAR Message[256];
	DWORD Options;
} BINDER_MESSAGE;

typedef struct {
	DWORD VersionOfBinder;
	DWORD SizeOfFile;			// size of binded file
	CHAR ExtensionOfFile[8];	// extension of binded file
} BINDER_FILE_HEADER;

typedef struct {
	BINDER_OPTIONS Options; // 128
	BINDER_MESSAGE Message; // 648
} BINDER_HEADER;
