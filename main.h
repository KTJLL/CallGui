#ifndef _MAIN_H
#define _MAIN_H_

#include <windows.h>
#include <ntsecapi.h>
#include <commctrl.h>

#include "defines.h"

enum {
	ID_TAB_CONTROL_MAIN = 0x0000,
	ID_TAB_CONTROL_KERNELHOOK,

	ID_PAGE_BASE_MAIN = 0x0002,
	ID_PAGE_PROCESS = 0x0002,
	ID_PAGE_KERNELHOOK,
	ID_PAGE_APPHOOK,

	ID_PAGE_BASE_KERNELHOOK = 0x0005,
	ID_PAGE_SSDT = 0x0005,
	ID_PAGE_SSSDT,
	ID_PAGE_OBJECT_HOOK, 

	ID_PROCESS_LISTVIEW = 0x0010,
	ID_INFO_TEXT,
	ID_THREAD_LISTVIEW,
	ID_HANDLE_LISTVIEW,
	ID_WINDOW_LISTVIEW,
	ID_MODULE_LISTVIEW,
	ID_SSDT_LISTVIEW,
	ID_SSSDT_LISTVIEW, 
	ID_OBJECT_HOOK_LISTVIEW
};

enum {
	MENU_REFRESH = 0x1000,
	MENU_PROCESS_TERMINATE,
	MENU_PROCESS_FORCE_TERMINATE,
	MENU_PROCESS_LOCK,
	MENU_PROCESS_SUSPEND,
	MENU_PROCESS_RESUME,
	MENU_THREAD_QUERY,
	MENU_THREAD_TERMINATE,
	MENU_THREAD_FORCE_TERMINATE,
	MENU_THREAD_SUSPEND,
	MENU_THREAD_RESUME,
	MENU_HANDLE_QUERY,
	MENU_HANDLE_CLOSE,
	MENU_WINDOW_QUERY,
	MENU_WINDOW_QUERY_INFORMATION,
	MENU_WINDOW_QUERY_CHILDWINDOW,
	MENU_WNDCTL_CLOSE,
	MENU_WNDCTL_SHOW,
	MENU_WNDCTL_HIDE,
	MENU_WNDCTL_FREEZE,
	MENU_WNDCTL_ENABLE,
	MENU_WNDCTL_MAXIMIZE,
	MENU_WNDCTL_MINIMIZE,
	MENU_WNDCTL_SETFORE,
	MENU_MODULE_QUERY,
	MENU_MODULE_UNLOAD,
	MENU_MODULE_GLOBAL_UNLOAD,
	MENU_MODULE_DELETE_FILE,
	MENU_MODULE_SHOW_DEPENT
};

#define UM_INITTHREADS (WM_USER+1)

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
        ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
                                                         )

#define FILE_DEVICE_UNKNOWN             0x00000022
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0

enum {
	OPEN_PROCESS_IOCTL_BASE = 0x800, 
	TERMINATE_PROCESS_IOCTL_BASE, 
	PROTECT_PROCESS_IOCTL_BASE, 
	QUERY_PROCESS_IOCTL_BASE, 
	QUERY_THREADS_IOCTL_BASE, 
	TERMINATE_THREAD_IOCTL_BASE, 
	QUERY_HANDLE_IOCTL_BASE, 
	CLOSE_HANDLE_IOCTL_BASE, 
	SUSPEND_PROCESS_IOCTL_BASE, 
	RESUME_PROCESS_IOCTL_BASE, 
	QUERY_SSDT_DATA_IOCTL_BASE, 
	GET_NTOSKRNL_BASE_IOCTL_BASE, 
	GET_KISERVICETABLE_BASE_IOCTL_BASE, 
	QUERY_SSSDT_DATA_IOCTL_BASE, 
	QUERY_OBJECT_TYPE_IOCTL_BASE, 
	QUERY_OBJECT_TYPE_CALLBACK_IOCTL_BASE
};

#define CTL_CODE_OPEN_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, OPEN_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_TERMINATE_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, TERMINATE_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_PROTECT_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, PROTECT_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_THREADS CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_THREADS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_TERMINATE_THREAD CTL_CODE(FILE_DEVICE_UNKNOWN, TERMINATE_THREAD_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_HANDLE CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_HANDLE_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_CLOSE_HANDLE CTL_CODE(FILE_DEVICE_UNKNOWN, CLOSE_HANDLE_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_SUSPEND_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, SUSPEND_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_RESUME_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, RESUME_PROCESS_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_SSDT_DATA CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_SSDT_DATA_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_GET_NTOSKRNL_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, GET_NTOSKRNL_BASE_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_GET_KISERVICETABLE_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, GET_KISERVICETABLE_BASE_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_SSSDT_DATA CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_SSSDT_DATA_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_OBJECT_TYPE_DATA CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_OBJECT_TYPE_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTL_CODE_QUERY_OBJECT_TYPE_CALLBACK CTL_CODE(FILE_DEVICE_UNKNOWN, QUERY_OBJECT_TYPE_CALLBACK_IOCTL_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)


extern HANDLE hSys;
extern HWND hThreadWnd, hMainWnd, hHandleWnd, hWindowWnd, hModuleWnd, hInfoTextWnd;
extern ULONG64 dwCurrentPID;
/*HFONT Font = CreateFont(-9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Shell Dlg");*/
extern HFONT Font;

//#pragma pack(push, 8)
typedef struct _PROCESS_INFO {
	//SIZE_T sizeOfInfo;
	ULONG64 dwPID;
	ULONG64 ParentPID;
	WCHAR imageName[64];
	//ULONG64 SessionID;
	ULONG64 EProcessAddr;
	//ULONG64 NumberOfThreads;
} PROCESS_INFO, *PPROCESS_INFO;

typedef struct _QUERY_PROCESS_INFORMATION {
	ULONG64 sizeOfInfo;
	UINT16 numberOfProcess;
	PROCESS_INFO ProcessInformation[1];
} QUERY_PROCESS_INFORMATION, *PQUERY_PROCESS_INFORMATION;
//#pragma pack(pop)

typedef struct _THREAD_INFO {
	ULONG64 threadId;
	//ULONG64 TebAddr;
	ULONG64 ThreadEntry;
	LONG32 basePriority;
	ULONG64 State;

} THREAD_INFO, *PTHREAD_INFO;

typedef struct _QUERY_THREAD_INFORMATION {
	ULONG64 sizeOfInfo;
	ULONG64 numberOfThreads;
	THREAD_INFO infos[1];
} QUERY_THREAD_INFORMATION, *PQUERY_THREAD_INFORMATION;

typedef struct _HANDLE_INFORMATION {
	UCHAR objectType;
	USHORT HandleValue;
	ULONG64 ObjectAddr;
	ULONG32 GrantedAccess;
} HANDLE_INFORMATION, *PHANDLE_INFORMATION;

typedef struct _QUERY_HANDLE_INFORMATION {
	ULONG64 sizeOfInfo;
	ULONG64 numberOfHandles;
	HANDLE_INFORMATION infos[1];
} QUERY_HANDLE_INFORMATION, *PQUERY_HANDLE_INFORMATION;

typedef struct _QUERY_SSDT_INFORMATION {
	ULONG64 KiServiceTableAddr;
	ULONG64 KernelBase;
	ULONG64 Addrs[SSDT_NUMBERS];
} QUERY_SSDT_INFORMATION, *PQUERY_SSDT_INFORMATION;

typedef struct _QUERY_SSSDT_INFORMATION{
	ULONG64 Win32kBase;
	ULONG64 w32pServiceTableAddr;
	ULONG64 Addrs[827];
} QUERY_SSSDT_INFORMATION, *PQUERY_SSSDT_INFORMATION;

typedef struct _QUERY_OBJECT_TYPE_INFORMATION{
	ULONG64 numberOfObjectTypes;
	WCHAR names[0x100][24];
} QUERY_OBJECT_TYPE_INFORMATION, *PQUERY_OBJECT_TYPE_INFORMATION;

typedef struct _CALLBACK_INFORMATION{
	ULONG64 bFromPsProcessType;
	ULONG64 PreCallback;
	ULONG64 PostCallback;
} CALLBACK_INFORMATION, *PCALLBACK_INFORMATION;

typedef struct _QUERY_OBJECT_TYPE_CALLBACK_INFORMATION{
	ULONG64 numberOfCallbacks;
	CALLBACK_INFORMATION infos[1];
} QUERY_OBJECT_TYPE_CALLBACK_INFORMATION, *PQUERY_OBJECT_TYPE_CALLBACK_INFORMATION;


/*
typedef enum SePrivilege_ {
        SeUnsolicitedInputPrivilege, // 0x00,
        SeUnknownInvalidPrivilege, // 0x01,
        SeCreateTokenPrivilege, // 0x02,
        SeAssignPrimaryTokenPrivilege, // 0x03,
        SeLockMemoryPrivilege, // 0x04,
        SeIncreaseQuotaPrivilege, // 0x05,
        SeMachineAccountPrivilege, // 0x06,
        SeTcbPrivilege, // 0x07,
        SeSecurityPrivilege, // 0x08,
        SeTakeOwnershipPrivilege, // 0x09,
        SeLoadDriverPrivilege, // 0x0A,
        SeSystemProfilePrivilege, // 0x0B,
        SeSystemtimePrivilege, // 0x0C,
        SeProfileSingleProcessPrivilege, // 0x0D,
        SeIncreaseBasePriorityPrivilege, // 0x0E,
        SeCreatePagefilePrivilege, // 0x0F,
        SeCreatePermanentPrivilege, // 0x10,
        SeBackupPrivilege, // 0x11,
        SeRestorePrivilege, // 0x12,
        SeShutdownPrivilege, // 0x13,
        SeDebugPrivilege, // 0x14,
        SeAuditPrivilege, // 0x15,
        SeSystemEnvironmentPrivilege, // 0x16,
        SeChangeNotifyPrivilege, // 0x17,
        SeRemoteShutdownPrivilege, // 0x18,
        SeUndockPrivilege, // 0x19,
        SeSyncAgentPrivilege, // 0x1A,
        SeEnableDelegationPrivilege, // 0x1B,
        SeManageVolumePrivilege, // 0x1C,
        SeImpersonatePrivilege, // 0x1D,
        SeCreateGlobalPrivilege, // 0x1E,
        SeTrustedCredManAccessPrivilege, // 0x1F,
        SeRelabelPrivilege, // 0x20,
        SeIncreaseWorkingSetPrivilege, // 0x21,
        SeTimeZonePrivilege, // 0x22,
        SeCreateSymbolicLinkPrivilege, // 0x23,
        SeDelegateSessionUserImpersonatePrivilege // =0x24
};*/
#define SeLoadDriverPrivilege 0x0A
#define NT_SUCCESS(status) (((NTSTATUS)(status)) >= 0)

typedef NTSTATUS(NTAPI* _NtLoadDriver)(PUNICODE_STRING);
typedef NTSTATUS(NTAPI* _NtUnLoadDriver)(PUNICODE_STRING);
//BOOL QueryFullProcessImageNameA(HANDLE, DWORD, LPSTR, PDWORD);
typedef NTSTATUS(WINAPI* _QueryFullProcessImageNameA)(HANDLE, DWORD, LPSTR, PDWORD);
typedef NTSTATUS(WINAPI* _RtlAdjustPrivilege)(ULONG, BOOL, BOOL, PBOOL);


LRESULT CALLBACK mainWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndPageProcessProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
ULONG64 GetDwordByISelectItem(HWND hwnd, int iSelectItem, int iSubItem);
BOOL ShowMenu(HWND hwnd, HMENU hMenu, LPARAM lParam);
BOOL AppendColumn(HWND hwndListView, LPLVCOLUMNA pColumn, LPCSTR name, INT cx);
BOOL ShowWindowWithLock(HWND hwndCur, HWND hwndTar, LPCSTR name, HWND hwndListView, INT iSelectItem, INT itemIndex);
BOOL QueryProcess(HWND hwndListView);
BOOL QueryThreads(HWND hwndListView, ULONG64 pid);
BOOL QueryHandles(HWND hwndListView, ULONG64 pid);
BOOL CALLBACK WndEnumProc(HWND Currenthwnd, LPARAM lParam);
BOOL QueryWindows(HWND hwndListview, ULONG64 pid);
LRESULT CALLBACK windowWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK handleWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK threadWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK windowInfoWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
//BOOL QueryWindowData(HWND hwnd);
void ReleaseReg(HKEY hKey);
LRESULT CALLBACK WndPageKernelHookProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL QuerySSDTFunctions(HWND hwndListView);
BOOL GetOriginDataFromNtOsKrnlImage();
BOOL GetOriginSSSDTData();

#endif