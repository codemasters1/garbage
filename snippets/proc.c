#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		puts("Returns address of procedure in given module. If module is not loaded, it tries to load it via LoadLibrary.\n");
		puts("usage: proc <module_name> <proc_name>\n");
		return 1;
	}
	
	BOOL loaded = FALSE;
	HMODULE module = GetModuleHandle(argv[1]);
	
	if (module == NULL)
	{
		module = LoadLibrary(argv[1]);
		
		if (module == NULL)
		{
			puts("error: could not find module handle\n");
			return 1;
		}
		
		loaded = TRUE;
	}
	
	FARPROC proc = GetProcAddress(module, argv[2]);
	
	if (loaded)
	{
		FreeLibrary(module);
	}
	
	if (proc == NULL)
	{
		puts("error: could not get address of procedure\n");
		return 1;
	}
	
	printf("Address of %s in module %s: 0x%1.8X\n", argv[2], argv[1], proc);
}