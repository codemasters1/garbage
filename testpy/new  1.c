#include <windows.h>
#include <PluginAPI.h>

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

TPluginLink PluginLink;
TPluginInfo PluginInfo;

INT_PTR __stdcall OnAddLine(WPARAM wParam, LPARAM lParam);

INT_PTR __stdcall OnAddLine(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

INT_PTR __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
	PluginLink = *Link;
  
	//PluginLink.HookEvent(AQQ_CONTACTS_ADDLINE,OnAddLine);
  
	if(PluginLink.CallService(AQQ_SYSTEM_MODULESLOADED,0,0));

	return 0;
}

INT_PTR __declspec(dllexport) __stdcall Unload()
{
	//PluginLink.UnhookEvent(OnWindowEvent);
	return 0;
}

PPluginInfo __declspec(dllexport) __stdcall AQQPluginInfo(DWORD AQQVersion)
{
	PluginInfo.cbSize = sizeof(TPluginInfo);
	PluginInfo.ShortName = L"xxx";
	PluginInfo.Version = PLUGIN_MAKE_VERSION(0,0,0,1);
	PluginInfo.Description = L"xxxx";
	PluginInfo.Author = L"darksv";
	PluginInfo.AuthorMail = L"";
	PluginInfo.Copyright = L"Autor Autor";
	PluginInfo.Homepage = L"";
	PluginInfo.Flag = 0;
	PluginInfo.ReplaceDefaultModule = 0;

	return &PluginInfo;
}