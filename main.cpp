#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <cwchar>
#include <malloc.h>
#include <ntsecapi.h>
#include <psapi.h>
#include <time.h>

#include "defines.h"
#include "main.h"

HANDLE hSys = INVALID_HANDLE_VALUE;
HWND hThreadWnd, hMainWnd, hHandleWnd, hWindowWnd, hModuleWnd, hInfoTextWnd;
ULONG64 dwCurrentPID = 0;
/*HFONT Font = CreateFont(-9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Shell Dlg");*/
HFONT Font = CreateFont(-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE, "Sim Sun");

ULONG64 GetDwordByISelectItem(HWND hwnd, int iSelectItem, int iSubItem) {
	CHAR szPID[260];
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iSelectItem;
	lvi.iSubItem = iSubItem;
	lvi.pszText = szPID;
	lvi.cchTextMax = 260 * sizeof(CHAR);
	SendMessage(hwnd, LVM_GETITEM, 0, (LPARAM)&lvi);
	ULONG64 dwPID;
	sscanf(szPID, "%lld", &dwPID);
	return dwPID;
}

ULONG64 Get64ByScannerISelectItem(HWND hwnd, int iSelectItem, int iSubItem, LPCSTR ScannerFormat) {
	CHAR String[260];
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iSelectItem;
	lvi.iSubItem = iSubItem;
	lvi.pszText = String;
	lvi.cchTextMax = 260 * sizeof(CHAR);
	SendMessage(hwnd, LVM_GETITEM, 0, (LPARAM)&lvi);
	ULONG64 Data = 0;
	sscanf(String, ScannerFormat, &Data);
	return Data;
}

BOOL ShowMenu(HWND hwnd, HMENU hMenu, LPARAM lParam) {
	LPNMHDR pNmh = (LPNMHDR)(lParam);
	LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)lParam;
	POINT point = pia->ptAction;
	ClientToScreen(pNmh->hwndFrom, &point);

	DWORD command = TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, \
	                                 point.x, point.y, hwnd, NULL);
	DestroyMenu(hMenu);

	if (command) {
		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(command, 0), 0);
	}
	return TRUE;
}

BOOL AppendColumn(HWND hwndListView, LPLVCOLUMNA pColumn, LPCSTR name, INT cx) {
	pColumn->pszText = (LPSTR)name;
	pColumn->cx = cx;
	SendMessage(hwndListView, LVM_INSERTCOLUMN, pColumn->iSubItem, (LPARAM)pColumn);
	pColumn->iSubItem ++;
	return TRUE;
}

BOOL ShowWindowWithLock(HWND hwndCur, HWND hwndTar, LPCSTR name1, LPCSTR name2, HWND hwndListView, INT iSelectItem, INT itemIndex) {
	if (iSelectItem == -1) return FALSE;
	ULONG64 dwID = GetDwordByISelectItem(hwndListView, iSelectItem, itemIndex);
	if (!dwID) return FALSE;

	SendMessage(hwndTar, UM_INITTHREADS, 0, dwID);
	ShowWindow(hwndTar, SW_SHOW);
	EnableWindow(hwndCur, FALSE);
	SetForegroundWindow(hwndTar);
	CHAR Full[MAX_PATH], temp1[MAX_PATH];
	sprintf(temp1, "[%ld]的", dwID);
	strcpy(Full, name1);
	strcat(Full, temp1);
	strcat(Full, name2);
	strcat(Full, "信息");
	SetWindowText(hwndTar, Full);
	return TRUE;
}

BOOL GetProcessFullImageName(HANDLE hProcess, _QueryFullProcessImageNameA QueryFullProcessImageNameA, LPSTR ImagePath, DWORD dwSize) {
	BOOL bOldImage = !QueryFullProcessImageNameA;
	ZeroMemory(ImagePath, MAX_PATH);
	if (bOldImage) {
		if (!GetModuleFileNameExA(hProcess, NULL, ImagePath, MAX_PATH * sizeof(WCHAR))) {
			return FALSE;
		}
	} else {
		QueryFullProcessImageNameA(hProcess, 0, ImagePath, &dwSize);
	}
	return TRUE;
}

BOOL AddProcessImageList(HIMAGELIST hImageList, HANDLE hProcess, _QueryFullProcessImageNameA QueryFullProcessImageNameA, LPSTR ImagePath) {
	HICON SmallIcon;
	/*DWORD dwSize = MAX_PATH;
	CHAR ImagePath[MAX_PATH];*/

	if (!hProcess) {
		SmallIcon = LoadIconA(NULL, IDI_APPLICATION);
		//ImageList_ReplaceIcon(hImageList, index, SmallIcon);
		ImageList_AddIcon(hImageList, SmallIcon);
		//SendMessageW(hwndListView, LVM_INSERTITEMW, 0, (LPARAM)&lviw);
	} else {
		SmallIcon = NULL;
		//GetProcessFullImageName(hProcess, QueryFullProcessImageNameA, ImagePath, dwSize);
		ExtractIconExA(ImagePath, 0, NULL, &SmallIcon, 1);
		if (!SmallIcon) {
			/*CHAR Buffer[MAX_PATH];
			sprintf(Buffer, "%ld  %s", GetLastError(), ImagePath);
			MessageBox(NULL, Buffer, "2", MB_OK);*/
			SmallIcon = LoadIconA(NULL, IDI_APPLICATION);
		}
		ImageList_AddIcon(hImageList, SmallIcon);
		//ImageList_ReplaceIcon(hImageList, index, SmallIcon);
		//SendMessageW(hwndListView, LVM_INSERTITEMW, 0, (LPARAM)&lviw);
	}
	return TRUE;
}

BOOL QueryProcess(HWND hwndListView) {
	clock_t time = clock();
	printf("[ INFO ] Start process query\n");
	ULONG64 size = sizeof(QUERY_PROCESS_INFORMATION) + sizeof(PROCESS_INFO) * 1023;
	PQUERY_PROCESS_INFORMATION pQpin = (PQUERY_PROCESS_INFORMATION)malloc(size);
	DWORD dwReturn;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_PROCESS, NULL, 0, (PVOID)pQpin, size, &dwReturn, NULL)) {
		free(pQpin);
		SetWindowText(hInfoTextWnd, "进程: 0");
		ShowWindow(hwndListView, SW_SHOW);
		printf("[ ERROR ] Cannot communicate with the driver! Use %.0f ms\n", difftime(clock(), time));
		return FALSE;
	}
	LVITEMW lviw;
	lviw.mask = LVIF_TEXT | LVIF_IMAGE;
	lviw.iItem = 0;
	lviw.iSubItem = 0;
	lviw.iImage = 0;
	HIMAGELIST hImageList = (HIMAGELIST)SendMessage(hwndListView, LVM_GETIMAGELIST, LVSIL_SMALL, 0);
	if (hImageList) {
		ImageList_Destroy(hImageList);
	}
	hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 0);
	SendMessage(hwndListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImageList);
	CHAR Buffer[260];
	USHORT index = 0;
	HMODULE hExe = NULL;
	HANDLE hProcess;
	CHAR ImagePath[MAX_PATH];
	HICON SmallIcon;
	DWORD dwSize;
	//ImageList_SetImageCount(hImageList, pQpin->numberOfProcess);
	_QueryFullProcessImageNameA QueryFullProcessImageNameA = (_QueryFullProcessImageNameA)GetProcAddress(GetModuleHandle("kernel32.dll"), "QueryFullProcessImageNameA");
	/*BOOL bOldImage = FALSE;
	if (!QueryFullProcessImageNameA){
		bOldImage = TRUE;
	}*/
	while (index < pQpin->numberOfProcess) {
		lviw.pszText = pQpin->ProcessInformation[index].imageName;	// 映像名称+小图标
		hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pQpin->ProcessInformation[index].dwPID);
		GetProcessFullImageName(hProcess, QueryFullProcessImageNameA, ImagePath, MAX_PATH);
		AddProcessImageList(hImageList, hProcess, QueryFullProcessImageNameA, ImagePath);
		SendMessageW(hwndListView, LVM_INSERTITEMW, 0, (LPARAM)&lviw);
		CloseHandle(hProcess);

		lvi.iSubItem = 1;	// PID
		sprintf(Buffer, "%ld", pQpin->ProcessInformation[index].dwPID);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

		lvi.iSubItem ++;	// 父PID
		sprintf(Buffer, "%ld", pQpin->ProcessInformation[index].ParentPID);
		//lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

		lvi.iSubItem ++;	// 映像路径
		lvi.pszText = ImagePath;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

		lvi.iSubItem ++;	// EPROCESS
		sprintf(Buffer, "0x%016llX", pQpin->ProcessInformation[index].EProcessAddr);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

		index++;
		lviw.iItem ++;
		lviw.iImage = lviw.iItem;
		lvi.iItem ++;
	}
	CHAR TextBuffer[MAX_PATH];
	sprintf(TextBuffer, "进程: %d", pQpin->numberOfProcess);
	SetWindowText(hInfoTextWnd, TextBuffer);
	free(pQpin);
	ShowWindow(hwndListView, SW_SHOW);
	printf("[ INFO ] Query process done! Use %.0f ms\n", difftime(clock(), time));
	return TRUE;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_TAB_CONTROL_MAIN && pNmh->code == TCN_SELCHANGE) {
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_PROCESS), SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_KERNELHOOK), SW_HIDE);
				HWND hwndTab = GetDlgItem(hwnd, ID_TAB_CONTROL_MAIN);

				int iCurrentPage = (int)SendMessage(hwndTab, TCM_GETCURSEL, 0, 0);
				switch (ID_PAGE_BASE_MAIN + iCurrentPage) {
					case ID_PAGE_PROCESS: {
						QueryProcess(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_PROCESS), ID_PROCESS_LISTVIEW));
						break;
					}
					case ID_PAGE_KERNELHOOK: {
						SendMessage(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_KERNELHOOK), ID_TAB_CONTROL_KERNELHOOK), TCM_SETCURSEL, 0, 0);
						QuerySSDTFunctions(
							GetDlgItem(
								GetDlgItem(
									GetDlgItem(
										hwnd, 
										ID_PAGE_KERNELHOOK), 
									ID_PAGE_SSDT), 
							ID_SSDT_LISTVIEW)
						);
						break;
					}
				}
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_BASE_MAIN + iCurrentPage), SW_SHOW);

				break;
			}
			break;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

BOOL QuerySSDTFunctions(HWND hwndListView) {
	//ULONG64 size = sizeof(ULONG64)*SSDT_NUMBERS;
	//PQUERY_SSDT_INFORMATION pQsin = (PQUERY_SSDT_INFORMATION)malloc(size);
	QUERY_SSDT_INFORMATION pQsin;
	DWORD dwReturn;
	DWORD64 ssdtnumbers = SSDT_NUMBERS;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_SSDT_DATA, (PVOID)&ssdtnumbers, sizeof(DWORD64), (PVOID)&pQsin, sizeof(QUERY_SSDT_INFORMATION), &dwReturn, NULL)) {
		//free(pQsin);
		SetWindowText(hInfoTextWnd, "SSDT函数: 0 被挂钩函数: 0");
		ShowWindow(hwndListView, SW_SHOW);
		return FALSE;
	}
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	CHAR Buffer[260];
	USHORT index = 0;
	DWORD dwSize;
	USHORT hookedCount = 0;
	while (index < SSDT_NUMBERS) {
		lvi.iSubItem = 0;
		sprintf(Buffer, "%d", index);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "Nt%s", ssdtfunc[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "0x%016llX", OriginSSDTData[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		if (pQsin.Addrs[index] != OriginSSDTData[index]) {
			strcpy(Buffer, "ssdt hook");
			hookedCount ++;
		} else {
			strcpy(Buffer, "-");
		}
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "0x%016llX", pQsin.Addrs[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		index++;
		lvi.iItem ++;
	}
	//free(pQsin);
	sprintf(Buffer, "SSDT函数: %d 被挂钩函数: %d", SSDT_NUMBERS, hookedCount);
	SetWindowText(hInfoTextWnd, Buffer);
	ShowWindow(hwndListView, SW_SHOW);
	return TRUE;
}

BOOL QuerySSSDTFunctions(HWND hwndListView) {
	QUERY_SSSDT_INFORMATION pQsin;
	DWORD dwReturn;
	DWORD64 sssdtnumbers = SSSDT_NUMBERS;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_SSSDT_DATA, (PVOID)&sssdtnumbers, sizeof(DWORD64), (PVOID)&pQsin, sizeof(QUERY_SSSDT_INFORMATION), &dwReturn, NULL)) {
		SetWindowText(hInfoTextWnd, "SSSDT函数: 0 被挂钩函数: 0");
		ShowWindow(hwndListView, SW_SHOW);
		return FALSE;
	}
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	CHAR Buffer[260];
	USHORT index = 0;
	DWORD dwSize;
	USHORT hookedCount = 0;
	while (index < SSSDT_NUMBERS) {
		lvi.iSubItem = 0;
		sprintf(Buffer, "%d", index);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "Nt%s", sssdtfunc[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "0x%016llX", OriginSSSDTData[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		if (pQsin.Addrs[index] != OriginSSSDTData[index]) {
			strcpy(Buffer, "ssdt hook");
			hookedCount ++;
		} else {
			strcpy(Buffer, "-");
		}
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		lvi.iSubItem ++;
		sprintf(Buffer, "0x%016llX", pQsin.Addrs[index]);
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		//printf("Nt%s 0x%016llx\n", sssdtfunc[index], pQsin.Addrs[index]);
		index++;
		lvi.iItem ++;
		
	}
	//free(pQsin);
	sprintf(Buffer, "SSSDT函数: %d 被挂钩函数: %d", SSSDT_NUMBERS, hookedCount);
	SetWindowText(hInfoTextWnd, Buffer);
	ShowWindow(hwndListView, SW_SHOW);
	return TRUE;
}

BOOL QueryObjectHook(HWND hwndListView){
	QUERY_OBJECT_TYPE_CALLBACK_INFORMATION pQoin;
	DWORD dwReturn;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_OBJECT_TYPE_CALLBACK, NULL, 0, (PVOID)&pQoin, sizeof(QUERY_OBJECT_TYPE_CALLBACK_INFORMATION), &dwReturn, NULL)) {
		SetWindowText(hInfoTextWnd, "ObjectHook个数: 0 ObjectType_Callback个数: 0");
		ShowWindow(hwndListView, SW_SHOW);
		return FALSE;
	}
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	CHAR Buffer[260];
	lvi.pszText = Buffer;
	memset(Buffer, 0, sizeof(Buffer));
	USHORT index = 0;
	DWORD dwSize;
	while (index < pQoin.numberOfCallbacks) {
		for(int i=0;i<2;i++){
			ULONG64 Addr = (i == 0 ? pQoin.infos[index].PostCallback : pQoin.infos[index].PreCallback);
			if(!Addr){
				continue;
			}
			lvi.iSubItem = 0;
			strcpy(Buffer, "-");
			SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, "-");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			strcpy(Buffer, "ObjectType_Callback");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, "0x%016llX", Addr);
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, "PsProcessType");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			index++;
			lvi.iItem ++;
		}
	}
	
	while (index < 47) {
		for(int i=0;i<8;i++){
			if(!(procedureNames[index].ProcedureName[i])){
				continue;
			}
			lvi.iSubItem = 0;
			strcpy(Buffer, procedureNames[index].ProcedureName[i]);
			SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, "-");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			strcpy(Buffer, "ObjectHook");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, "0x%016llX", 0);
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iSubItem ++;
			sprintf(Buffer, procedureNames[index].ProcedureName[i]);
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
			lvi.iItem ++;
		}

		index++;
	}
	
	//free(pQsin);
	sprintf(Buffer, "ObjectHook个数: %d ObjectType_Callback个数: %d", 0, (ULONG)pQoin.numberOfCallbacks);
	SetWindowText(hInfoTextWnd, Buffer);
	ShowWindow(hwndListView, SW_SHOW);
	return TRUE;
}

LRESULT CALLBACK WndPageSSDTProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_SSDT_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "恢复");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编原始地址");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编当前地址");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					HWND hSSDTListView = GetDlgItem(hwnd, ID_SSDT_LISTVIEW);
					QuerySSDTFunctions(hSSDTListView);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK WndPageSSSDTProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_SSSDT_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "恢复");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编原始地址");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编当前地址");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					HWND hSSSDTListView = GetDlgItem(hwnd, ID_SSSDT_LISTVIEW);
					QuerySSSDTFunctions(hSSSDTListView);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK WndPageObjectHook(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == 0 && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "恢复");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编原始地址");
				AppendMenu(hMenu, MF_STRING | MF_DISABLED, 0, "反汇编当前地址");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK WndPageKernelHookProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_TAB_CONTROL_KERNELHOOK && pNmh->code == TCN_SELCHANGE) {
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_SSDT), SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_SSSDT), SW_HIDE);
				HWND hwndTab = GetDlgItem(hwnd, ID_TAB_CONTROL_KERNELHOOK);

				int iCurrentPage = (int)SendMessage(hwndTab, TCM_GETCURSEL, 0, 0);
				/*if (iCurrentPage + ID_PAGE_BASE_KERNELHOOK == ID_PAGE_SSDT) {
					//ChoseSsdtPage = TRUE;
					QuerySSDTFunctions(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_SSDT), ID_SSDT_LISTVIEW));
				} else {
					//ChoseSsdtPage = FALSE;
					QuerySSSDTFunctions(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_SSSDT), ID_SSSDT_LISTVIEW));
				}*/
				switch (iCurrentPage + ID_PAGE_BASE_KERNELHOOK) {
					case ID_PAGE_SSDT:
						QuerySSDTFunctions(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_SSDT), ID_SSDT_LISTVIEW));
						break;
					case ID_PAGE_SSSDT:
						QuerySSSDTFunctions(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_SSSDT), ID_SSSDT_LISTVIEW));
						break;
					case ID_PAGE_OBJECT_HOOK:
						QueryObjectHook(GetDlgItem(GetDlgItem(hwnd, ID_PAGE_OBJECT_HOOK), ID_OBJECT_HOOK_LISTVIEW));
					default:
						//TODO
						break;
				}
				ShowWindow(GetDlgItem(hwnd, ID_PAGE_BASE_KERNELHOOK + iCurrentPage), SW_SHOW);
				break;
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK WndPageProcessProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	/*if (Message == WM_COMMAND || Message == WM_CONTEXTMENU || Message == WM_NOTIFY) {
	    return mainWndProc(hwnd, Message, wParam, lParam);
	}*/
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_PROCESS_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新(&R)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_PROCESS_TERMINATE, "结束(&T)");
				AppendMenu(hMenu, MF_STRING, MENU_PROCESS_FORCE_TERMINATE, "强制结束(&F)");
				AppendMenu(hMenu, MF_STRING, MENU_PROCESS_LOCK, "锁定(&L)");
				AppendMenu(hMenu, MF_STRING, MENU_PROCESS_SUSPEND, "挂起(&S)");
				AppendMenu(hMenu, MF_STRING, MENU_PROCESS_RESUME, "恢复(&E)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_THREAD_QUERY, "查看线程信息");
				AppendMenu(hMenu, MF_STRING, MENU_HANDLE_QUERY, "查看句柄信息");
				AppendMenu(hMenu, MF_STRING, MENU_WINDOW_QUERY, "查询窗口信息");
				AppendMenu(hMenu, MF_STRING, MENU_MODULE_QUERY, "查询模块信息");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
		}
		case WM_COMMAND: {
			HWND hProcessListView = GetDlgItem(hwnd, ID_PROCESS_LISTVIEW);
			int iSelectItem = SendMessage(hProcessListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					QueryProcess(GetDlgItem(hwnd, ID_PROCESS_LISTVIEW));
					break;
				}
				case MENU_PROCESS_TERMINATE: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要结束它吗？结束系统进程可能导致系统不稳定!", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwPID = GetDwordByISelectItem(hProcessListView, iSelectItem, 1);
						if (!dwPID) break;

						if (dwPID == GetCurrentProcessId()) {
							PostQuitMessage(0);
							break;
						}

						//ULONG64 hProcess = 0;
						//DWORD dwReturn = 0;
						//if (!DeviceIoControl(hSys, CTL_CODE_OPEN_PROCESS, &dwPID, sizeof(ULONG64), &hProcess, sizeof(ULONG64), &dwReturn, NULL)){
						//	break;
						//}
						CHAR Buffer[MAX_PATH];
						HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPID);
						if (!TerminateProcess((HANDLE)hProcess, 0)) {
							sprintf(Buffer, "结束进程失败! 错误码: %d", GetLastError());
							MessageBox(hwnd, Buffer, "进程", MB_OK | MB_ICONERROR);
							CloseHandle(hProcess);
							break;
						}
						CloseHandle(hProcess);
						MessageBox(hwnd, "结束进程成功", "进程", MB_OK | MB_ICONINFORMATION);
						//QueryProcess(hProcessListView);
						SendMessage(hProcessListView, LVM_DELETEITEM, iSelectItem, 0);
					}
					break;
				}
				case MENU_PROCESS_FORCE_TERMINATE: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要结束它吗？结束系统进程可能导致系统不稳定!", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwPID = GetDwordByISelectItem(hProcessListView, iSelectItem, 1);
						if (!dwPID) break;

						if (dwPID == GetCurrentProcessId()) {
							PostQuitMessage(0);
							break;
						}

						DWORD dwReturn = 0;
						CHAR Buffer[MAX_PATH];
						if (!DeviceIoControl(hSys, CTL_CODE_TERMINATE_PROCESS, &dwPID, sizeof(ULONG64), NULL, 0, &dwReturn, NULL)) {
							sprintf(Buffer, "强制结束进程失败! 返回状态: %d", dwReturn);
							MessageBox(hwnd, Buffer, "进程", MB_OK | MB_ICONERROR);
							break;
						}
						MessageBox(hwnd, "强制结束进程成功", "进程", MB_OK | MB_ICONINFORMATION);
						//QueryProcess(hProcessListView);
						SendMessage(hProcessListView, LVM_DELETEITEM, iSelectItem, 0);
					}
					break;
				}
				case MENU_PROCESS_LOCK: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要锁定它吗?", "", MB_YESNO) == IDYES) {
						ULONG64 dwPID = GetDwordByISelectItem(hProcessListView, iSelectItem, 1);
						if (!dwPID) break;

						DWORD dwReturn = 0;
						CHAR Buffer[MAX_PATH];
						if (!DeviceIoControl(hSys, CTL_CODE_PROTECT_PROCESS, &dwPID, sizeof(ULONG64), NULL, 0, &dwReturn, NULL)) {
							sprintf(Buffer, "锁定进程失败! 返回状态: %d", dwReturn);
							MessageBox(hwnd, Buffer, "进程", MB_OK | MB_ICONERROR);
							break;
						}
						MessageBox(hwnd, "锁定进程成功", "进程", MB_OK | MB_ICONINFORMATION);
						//QueryProcess(hProcessListView);
					}
					break;
				}
				case MENU_PROCESS_SUSPEND: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要挂起它吗?挂起系统进程可能导致系统不稳定!", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwPID = GetDwordByISelectItem(hProcessListView, iSelectItem, 1);
						if (!dwPID) break;

						DWORD dwReturn;
						CHAR Buffer[MAX_PATH];
						if (!DeviceIoControl(hSys, CTL_CODE_SUSPEND_PROCESS, &dwPID, sizeof(ULONG64), NULL, 0, &dwReturn, NULL)) {
							sprintf(Buffer, "挂起进程失败! 返回状态: %d", dwReturn);
							MessageBox(hwnd, Buffer, "进程", MB_OK | MB_ICONERROR);
							break;
						}
						MessageBox(hwnd, "挂起进程成功", "进程", MB_OK | MB_ICONINFORMATION);
						QueryProcess(GetDlgItem(hwnd, ID_PROCESS_LISTVIEW));
					}
					break;
				}
				case MENU_PROCESS_RESUME: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要恢复它吗?恢复系统进程可能导致系统不稳定!", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwPID = GetDwordByISelectItem(hProcessListView, iSelectItem, 1);
						if (!dwPID) break;

						DWORD dwReturn;
						CHAR Buffer[MAX_PATH];
						if (!DeviceIoControl(hSys, CTL_CODE_RESUME_PROCESS, &dwPID, sizeof(ULONG64), NULL, 0, &dwReturn, NULL)) {
							sprintf(Buffer, "恢复进程失败! 返回状态: %d", dwReturn);
							MessageBox(hwnd, Buffer, "进程", MB_OK | MB_ICONERROR);
							break;
						}
						MessageBox(hwnd, "恢复进程成功", "进程", MB_OK | MB_ICONINFORMATION);
						QueryProcess(GetDlgItem(hwnd, ID_PROCESS_LISTVIEW));
					}
				}
				case MENU_THREAD_QUERY: {
					ShowWindowWithLock(hMainWnd, hThreadWnd, "进程", "线程", hProcessListView, iSelectItem, 1);
					break;
				}
				case MENU_HANDLE_QUERY: {
					ShowWindowWithLock(hMainWnd, hHandleWnd, "进程", "句柄", hProcessListView, iSelectItem, 1);
					break;
				}
				case MENU_WINDOW_QUERY: {
					ShowWindowWithLock(hMainWnd, hWindowWnd, "进程", "窗口", hProcessListView, iSelectItem, 1);
					break;
				}
				case MENU_MODULE_QUERY: {
					ShowWindowWithLock(hMainWnd, hModuleWnd, "进程", "模块", hProcessListView, iSelectItem, 1);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

static LPCSTR threadState[10] = {"已初始化", "就绪", "运行", "即将运行", "终止", "等待", "状态切换", "延迟就绪", "等待门对象"};
BOOL QueryThreads(HWND hwndListView, ULONG64 pid) {
	ULONG64 size = sizeof(QUERY_THREAD_INFORMATION) + 1023 * sizeof(THREAD_INFO);
	PQUERY_THREAD_INFORMATION qTpin = (PQUERY_THREAD_INFORMATION)malloc(size);
	if (!qTpin) return FALSE;
	DWORD dwReturn;
	ULONG64 dwPid = pid;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_THREADS, &dwPid, sizeof(ULONG64), qTpin, size, &dwReturn, NULL)) {
		free(qTpin);
		ShowWindow(hwndListView, SW_SHOW);
		return FALSE;
	}
	LVITEM lvi;
	CHAR Buffer[260];
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	USHORT index = 0;
	while (index < qTpin->numberOfThreads) {
		sprintf(Buffer, "%lu", qTpin->infos[index].threadId);
		lvi.iSubItem = 0;
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
		sprintf(Buffer, "0x%016llX", qTpin->infos[index].ThreadEntry);
		lvi.iSubItem = 1;
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		sprintf(Buffer, "%ld", qTpin->infos[index].basePriority);
		lvi.iSubItem = 2;
		lvi.pszText = Buffer;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		//sprintf(Buffer, "%ld", qTpin->infos[index].State);
		lvi.iSubItem = 3;
		//lvi.pszText = Buffer;
		if (qTpin->infos[index].State < 10) {
			lvi.pszText = (LPSTR)threadState[qTpin->infos[index].State];
		} else {
			lvi.pszText = (LPSTR)"未知状态";
		}
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		index++;
		lvi.iItem++;
	}
	free(qTpin);
	ShowWindow(hwndListView, SW_SHOW);
	return TRUE;
}

BOOL QueryHandles(HWND hwndListView, ULONG64 pid) {
	DWORD dwReturn;
	ULONG64 ObjectDataSize = sizeof(QUERY_OBJECT_TYPE_INFORMATION);
	PQUERY_OBJECT_TYPE_INFORMATION qOpin = (PQUERY_OBJECT_TYPE_INFORMATION)malloc(ObjectDataSize);
	if (!qOpin) return FALSE;
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_OBJECT_TYPE_DATA, NULL, 0, qOpin, ObjectDataSize, &dwReturn, NULL)) {
		free(qOpin);
		return FALSE;
	}
	
	ULONG64 size = sizeof(QUERY_HANDLE_INFORMATION) + 1023 * sizeof(HANDLE_INFORMATION);
	PQUERY_HANDLE_INFORMATION qHpin = (PQUERY_HANDLE_INFORMATION)malloc(size);
	if (!qHpin) return FALSE;
	ULONG64 dwPid = pid;
	ShowWindow(hwndListView, SW_HIDE);
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_HANDLE, &dwPid, sizeof(ULONG64), qHpin, size, &dwReturn, NULL)) {
		free(qHpin);
		free(qOpin);
		ShowWindow(hwndListView, SW_SHOW);
		return FALSE;
	}
	LVITEM lvi;
	CHAR Buffer[260];
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	USHORT index = 0;
	lvi.pszText = Buffer;
	
	LVITEMW lviw;
	WCHAR WideBuffer[50];
	lviw.mask = LVIF_TEXT;
	lviw.iItem = 0;
	lviw.iSubItem = 3;
	lviw.pszText = WideBuffer;
	while (index < qHpin->numberOfHandles) {
		//sprintf(Buffer, "% hu", qHpin->infos[index].HandleValue);
		sprintf(Buffer, "0x%08X", qHpin->infos[index].HandleValue);
		lvi.iSubItem = 0;
		SendMessage(hwndListView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
		sprintf(Buffer, "0x%016llX", qHpin->infos[index].ObjectAddr);
		lvi.iSubItem = 1;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		sprintf(Buffer, "0x%08X", qHpin->infos[index].GrantedAccess);
		lvi.iSubItem = 2;
		SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		//sprintf(Buffer, "%ld", qTpin->infos[index].State);
		lvi.iSubItem = 3;
		//lvi.pszText = Buffer;
		/*if (qHpin->infos[index].objectType < 29){
			lvi.pszText = (LPSTR)handleTypes[qHpin->infos[index].objectType];
		} else {
			lvi.pszText = "未知类型";
		}*/
		UCHAR ObjectType = qHpin->infos[index].objectType;
		if (ObjectType < qOpin->numberOfObjectTypes && ObjectType >= 2){
			memcpy(WideBuffer, &qOpin->names[ObjectType-2], 24 * sizeof(WCHAR));
			//MessageBoxW(NULL, WideBuffer, L"", MB_OK);
			SendMessage(hwndListView, LVM_SETITEMW, 0, (LPARAM)&lviw);
		} else {
			strcpy(Buffer, "未知类型");
			SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);
		}
		//sprintf(Buffer, "%ld", qHpin->infos[index].objectType);
		
		index++;
		lvi.iItem++;
		lviw.iItem++;
	}
	free(qHpin);
	free(qOpin);
	ShowWindow(hwndListView, SW_SHOW);
	return TRUE;
}

typedef struct _EnumWindowData {
	ULONG64 pid;
	HWND hwndListView;
	LVITEM lvi;
	CHAR Buffer[260];
} EnumWindowData, *PEnumWindowData;

BOOL CALLBACK WndEnumProc(HWND Currenthwnd, LPARAM lParam) {
	DWORD currentPID, currentTID;
	currentTID = GetWindowThreadProcessId(Currenthwnd, &currentPID);
	if (currentPID != ((PEnumWindowData)lParam)->pid) return TRUE;
	HWND hwnd = ((PEnumWindowData)lParam)->hwndListView;
	LPLVITEMA lvi = &((PEnumWindowData)lParam)->lvi;
	LPSTR Buffer = (LPSTR)((PEnumWindowData)lParam)->Buffer;

	lvi->iSubItem = 0;
	sprintf(Buffer, "%d", currentPID);
	SendMessage(hwnd, LVM_INSERTITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	sprintf(Buffer, "%u", (ULONG64)Currenthwnd);
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	sprintf(Buffer, "%d", currentTID);
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	GetWindowText(Currenthwnd, Buffer, MAX_PATH);
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	GetClassName(Currenthwnd, Buffer, MAX_PATH);
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	sprintf(Buffer, "0x%08X", GetWindowLong(Currenthwnd, GWL_STYLE));
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	sprintf(Buffer, "0x%08X", GetWindowLong(Currenthwnd, GWL_EXSTYLE));
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	lvi->iSubItem ++;
	RECT rect;
	GetWindowRect(Currenthwnd, &rect);
	sprintf(Buffer, "%d x %d", rect.right - rect.left, rect.bottom - rect.top);
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)lvi);
	return TRUE;
}

BOOL QueryWindows(HWND hwndListview, ULONG64 pid) {
	EnumWindowData data;
	data.hwndListView = hwndListview;
	data.pid = pid;
	data.lvi.mask = LVIF_TEXT;
	data.lvi.iItem = 0;
	data.lvi.pszText = data.Buffer;
	ShowWindow(hwndListview, SW_HIDE);
	SendMessage(hwndListview, LVM_DELETEALLITEMS, 0, 0);
	EnumWindows(WndEnumProc, (LPARAM)&data);
	ShowWindow(hwndListview, SW_SHOW);
	return TRUE;
}
/*
BOOL ChildWndEnumProc(HWND hwnd, LPARAM lParam){
	HWND hwndListView = ((PEnumWindowData)lParam)->hwndListView;
	LPLVITEMA lvi = &((PEnumWindowData)lParam)->lvi;
	LPSTR Buffer = (LPSTR)((PEnumWindowData)lParam)->Buffer;

	lvi->iSubItem = 0;

}

BOOL QueryChildWindows(HWND hwndListView, HWND hwndTarget){
	EnumWindowData data;
	data.hwndListView = hwndListView;
	data.lvi.mask = LVIF_TEXT;
	data.lvi.iItem = 0;
	data.lvi.pszText = data.Buffer;
	SendMessage(hwndListView, LVM_DELETEALLITEMS, 0, 0);

}
*/
LRESULT CALLBACK windowWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom = ID_HANDLE_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新(&R)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				//AppendMenu(hMenu, MF_STRING, MENU_WINDOW_INFORMATION, "详细信息(&I)");
				AppendMenu(hMenu, MF_STRING, MENU_WINDOW_QUERY_CHILDWINDOW, "遍历子窗口(&C)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_CLOSE, "关闭窗口(&D)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_HIDE, "隐藏窗口(&H)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_SHOW, "显示窗口(&S)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_FREEZE, "冻结窗口(&F)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_ENABLE, "激活窗口(&E)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_MAXIMIZE, "最大化窗口(&A)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_MINIMIZE, "最小化窗口(&I)");
				AppendMenu(hMenu, MF_STRING, MENU_WNDCTL_SETFORE, "前置窗口(&G)");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			HWND hWindowListView = GetDlgItem(hwnd, ID_WINDOW_LISTVIEW);
			int iSelectItem = SendMessage(hWindowListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					QueryWindows(GetDlgItem(hwnd, ID_WINDOW_LISTVIEW), dwCurrentPID);
					break;
				}
				/*case MENU_WINDOW_QUERY_INFORMATION: {
					ShowWindowWithLock(hwnd, hWindowInfoWnd, "窗口", "详细", GetDlgItem(hwnd, ID_WINDOW_LISTVIEW), iSelectItem, 0);
					QueryWindowData(hWindowInfoWnd);
					break;
				}*/
				case MENU_WINDOW_QUERY_CHILDWINDOW: {
					break;
				}
				default: {
					if (iSelectItem == -1) break;
					HWND hwndTarget = (HWND)GetDwordByISelectItem(hWindowListView, iSelectItem, 1);
					if (!hwndTarget) break;
					CHAR Buffer[MAX_PATH];
					switch (LOWORD(wParam)) {
						case MENU_WNDCTL_CLOSE: {
							if (MessageBox(hwnd, "你真的要关闭这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								BOOL bOk = FALSE;
								bOk |= !SendMessage(hwndTarget, WM_CLOSE, 0, 0);
								bOk |= !SendMessage(hwndTarget, WM_DESTROY, 0, 0);
								bOk |= !SendMessage(hwndTarget, WM_NCDESTROY, 0, 0);
								bOk |= CloseWindow(hwndTarget);
								bOk |= DestroyWindow(hwndTarget);
								if (!bOk) {
									sprintf(Buffer, "关闭失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "关闭成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_HIDE: {
							if (MessageBox(hwnd, "你真的要隐藏这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!ShowWindow(hwndTarget, SW_HIDE)) {
									sprintf(Buffer, "隐藏失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "隐藏成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_SHOW: {
							if (MessageBox(hwnd, "你真的要显示这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (ShowWindow(hwndTarget, SW_SHOW)) {
									sprintf(Buffer, "显示失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "显示成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_FREEZE: {
							if (MessageBox(hwnd, "你真的要冻结这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!EnableWindow(hwndTarget, FALSE)) {
									sprintf(Buffer, "冻结失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "冻结成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_ENABLE: {
							if (MessageBox(hwnd, "你真的要激活这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!EnableWindow(hwndTarget, TRUE)) {
									sprintf(Buffer, "激活失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "激活成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_MAXIMIZE: {
							if (MessageBox(hwnd, "你真的要最大化这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!ShowWindow(hwndTarget, SW_MAXIMIZE)) {
									sprintf(Buffer, "最大化失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "最大化成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_MINIMIZE: {
							if (MessageBox(hwnd, "你真的要最小化这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!ShowWindow(hwndTarget, SW_MINIMIZE)) {
									sprintf(Buffer, "最小化失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "最小化成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
						case MENU_WNDCTL_SETFORE: {
							if (MessageBox(hwnd, "你真的要前置这个窗口吗？", "窗口", MB_YESNO | MB_ICONWARNING) == IDYES) {
								if (!SetForegroundWindow(hwndTarget)) {
									sprintf(Buffer, "前置失败！错误码: %d", GetLastError());
									MessageBox(hwnd, Buffer, "窗口", MB_OK | MB_ICONERROR);
									break;
								}
								MessageBox(hwnd, "前置成功", "窗口", MB_OK | MB_ICONINFORMATION);
								QueryWindows(hWindowListView, dwCurrentPID);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case UM_INITTHREADS: {
			dwCurrentPID = lParam;
			QueryWindows(GetDlgItem(hwnd, ID_WINDOW_LISTVIEW), dwCurrentPID);
			break;
		}
		case WM_CLOSE: {
			EnableWindow(hMainWnd, TRUE);
			ShowWindow(hwnd, SW_HIDE);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK handleWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case UM_INITTHREADS: {
			dwCurrentPID = lParam;
			QueryHandles(GetDlgItem(hwnd, ID_HANDLE_LISTVIEW), dwCurrentPID);
			break;
		}
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom = ID_HANDLE_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新(&R)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_HANDLE_CLOSE, "关闭");
				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			HWND hHandleListView = GetDlgItem(hwnd, ID_HANDLE_LISTVIEW);
			int iSelectItem = SendMessage(hHandleListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					QueryHandles(hHandleListView, dwCurrentPID);
					break;
				}
				case MENU_HANDLE_CLOSE: {
					ULONG64 Parameters[2];
					Parameters[0] = dwCurrentPID;
					Parameters[1] = Get64ByScannerISelectItem(hHandleListView, iSelectItem, 0, "0x%X");
					DWORD dwReturn;
					if (!DeviceIoControl(hSys, CTL_CODE_CLOSE_HANDLE, (PVOID)Parameters, sizeof(Parameters), NULL, 0, &dwReturn, NULL)){
						break;
					}
					QueryHandles(hHandleListView, dwCurrentPID);
					break;
				}
			}
			break;
		}
		case WM_CLOSE: {
			EnableWindow(hMainWnd, TRUE);
			ShowWindow(hwnd, SW_HIDE);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

LRESULT CALLBACK threadWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case UM_INITTHREADS: {
			dwCurrentPID = lParam;
			QueryThreads(GetDlgItem(hwnd, ID_THREAD_LISTVIEW), dwCurrentPID);
			break;
		}
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_THREAD_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新(&R)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_THREAD_TERMINATE, "结束(&T)");
				AppendMenu(hMenu, MF_STRING, MENU_THREAD_FORCE_TERMINATE, "强制结束(&F)");
				AppendMenu(hMenu, MF_STRING, MENU_THREAD_SUSPEND, "挂起(&S)");
				AppendMenu(hMenu, MF_STRING, MENU_THREAD_RESUME, "恢复(&E)");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			HWND hThreadListView = GetDlgItem(hwnd, ID_THREAD_LISTVIEW);
			int iSelectItem = SendMessage(hThreadListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					QueryThreads(GetDlgItem(hwnd, ID_THREAD_LISTVIEW), dwCurrentPID);
					break;
				}
				case MENU_THREAD_TERMINATE: {
					if (iSelectItem == -1) break;
					ULONG64 dwTID = GetDwordByISelectItem(hThreadListView, iSelectItem, 0);
					if (!dwTID) break;

					if (dwTID == GetCurrentThreadId()) {
						PostQuitMessage(0);
						break;
					}

					if (MessageBox(NULL, "你真的要结束它吗?结束系统线程可能导致系统不稳定", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, dwTID);
						CHAR Buffer[MAX_PATH];
						if (!TerminateThread(hThread, 0)) {
							sprintf(Buffer, "结束线程失败! 错误码: %d", GetLastError());
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							CloseHandle(hThread);
							break;
						}
						CloseHandle(hThread);
						MessageBox(hwnd, "结束线程成功", "线程", MB_OK | MB_ICONINFORMATION);
						QueryThreads(hThreadListView, dwCurrentPID);
					}
					break;
				}
				case MENU_THREAD_FORCE_TERMINATE: {
					if (iSelectItem == -1) break;
					ULONG64 dwTID = GetDwordByISelectItem(hThreadListView, iSelectItem, 0);
					if (!dwTID) break;

					if (dwTID == GetCurrentThreadId()) {
						PostQuitMessage(0);
						break;
					}

					if (MessageBox(hwnd, "你真的要结束它吗？这可能导致系统不稳定!", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						DWORD dwReturn;
						CHAR Buffer[MAX_PATH];
						if (!DeviceIoControl(hSys, CTL_CODE_TERMINATE_THREAD, &dwTID, sizeof(ULONG64), NULL, 0, &dwReturn, NULL)) {
							sprintf(Buffer, "强制结束线程失败! 返回状态: %d", dwReturn);
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							break;
						}
						MessageBox(hwnd, "强制结束线程成功", "线程", MB_OK | MB_ICONINFORMATION);
						QueryThreads(hThreadListView, dwCurrentPID);
					}
					break;
				}
				case MENU_THREAD_SUSPEND: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要挂起它吗？挂起系统线程可能导致系统不稳定", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwTID = GetDwordByISelectItem(hThreadListView, iSelectItem, 0);
						if (!dwTID) break;

						CHAR Buffer[MAX_PATH];

						HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTID);
						if (!hThread) {
							sprintf(Buffer, "挂起线程失败1! 错误码: %d", GetLastError());
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							break;
						}

						if (!SuspendThread(hThread)) {
							sprintf(Buffer, "挂起线程失败2! 错误码: %d", GetLastError());
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							CloseHandle(hThread);
							break;
						}
						CloseHandle(hThread);
						MessageBox(hwnd, "挂起线程成功", "线程", MB_OK | MB_ICONINFORMATION);
						QueryThreads(hThreadListView, dwCurrentPID);
					}
					break;
				}
				case MENU_THREAD_RESUME: {
					if (iSelectItem == -1) break;
					if (MessageBox(hwnd, "你真的要恢复它吗？恢复系统线程可能导致系统不稳定", "", MB_YESNO | MB_ICONWARNING) == IDYES) {
						ULONG64 dwTID = GetDwordByISelectItem(hThreadListView, iSelectItem, 0);
						if (!dwTID) break;

						CHAR Buffer[MAX_PATH];

						HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTID);
						if (!hThread) {
							sprintf(Buffer, "恢复线程失败1! 错误码: %d", GetLastError());
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							break;
						}

						if (!ResumeThread(hThread)) {
							sprintf(Buffer, "恢复线程失败2! 错误码: %d", GetLastError());
							CloseHandle(hThread);
							MessageBox(hwnd, Buffer, "线程", MB_OK | MB_ICONERROR);
							break;
						}
						CloseHandle(hThread);
						MessageBox(hwnd, "恢复线程成功", "线程", MB_OK | MB_ICONINFORMATION);
						QueryThreads(hThreadListView, dwCurrentPID);
					}
					break;
				}
			}
			break;
		}
		case WM_CLOSE: {
			EnableWindow(hMainWnd, TRUE);
			//SetForegroundWindow(hMainWnd);
			ShowWindow(hwnd, SW_HIDE);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK moduleWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_NOTIFY: {
			LPNMHDR pNmh = (LPNMHDR)lParam;
			if (pNmh->idFrom == ID_MODULE_LISTVIEW && pNmh->code == NM_RCLICK) {
				HMENU hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, MENU_REFRESH, "刷新(&R)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_MODULE_UNLOAD, "卸载(&U)");
				AppendMenu(hMenu, MF_STRING, MENU_MODULE_GLOBAL_UNLOAD, "全局卸载(&G)");
				AppendMenu(hMenu, MF_STRING, MENU_MODULE_DELETE_FILE, "删除模块文件(&F)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, "");
				AppendMenu(hMenu, MF_STRING, MENU_MODULE_SHOW_DEPENT, "查看全局依赖关系(&D)");

				ShowMenu(hwnd, hMenu, lParam);
				break;
			}
			break;
		}
		case WM_COMMAND: {
			HWND hModuleListView = GetDlgItem(hwnd, ID_MODULE_LISTVIEW);
			int iSelectItem = SendMessage(hModuleListView, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			switch (LOWORD(wParam)) {
				case MENU_REFRESH: {
					break;
				}
				case MENU_MODULE_UNLOAD: {
					if (iSelectItem == -1) break;
					break;
				}
				case MENU_MODULE_GLOBAL_UNLOAD: {
					if (iSelectItem == -1) break;
					break;
				}
				case MENU_MODULE_DELETE_FILE: {
					if (iSelectItem == -1) break;
					break;
				}
				case MENU_MODULE_SHOW_DEPENT: {
					if (iSelectItem == -1) break;
					break;
				}
			}
			break;
		}
		case WM_CLOSE: {
			EnableWindow(hMainWnd, TRUE);
			ShowWindow(hModuleWnd, SW_HIDE);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	}
	return 0;
}

void ReleaseReg(HKEY hKey) {
	RegDeleteValueW(hKey, L"ImagePath");
	RegDeleteValueW(hKey, L"Type");
	RegDeleteValueW(hKey, L"Start");
	RegDeleteValueW(hKey, L"ErrorControl");
	RegDeleteValueW(hKey, L"");
	HKEY hEnum;
	RegOpenKeyW(hKey, L"Enum", &hEnum);
	RegDeleteValueW(hEnum, L"0");
	RegDeleteValueW(hEnum, L"Count");
	RegDeleteValueW(hEnum, L"NextInstance");
	RegDeleteValueW(hEnum, L"");
	RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\KTJLL\\Enum");
	RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\KTJLL");
	RegCloseKey(hKey);
	RegCloseKey(hEnum);
}

HKEY InitReg(LPWSTR Path) {
	HKEY hKey;
	DWORD disposition;
	LONG result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\KTJLL", 0, NULL, \
	                              REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposition);
	if (result != ERROR_SUCCESS) {
		return 0;
	}

	DWORD dwValue;
	result |= RegSetValueExW(hKey, L"ImagePath", 0, REG_EXPAND_SZ, (const LPBYTE)Path, (wcslen(Path) + 1) * sizeof(WCHAR));
	dwValue = SERVICE_KERNEL_DRIVER;
	result |= RegSetValueExW(hKey, L"Type", 0, REG_DWORD, (const LPBYTE)&dwValue, sizeof(DWORD));
	dwValue = SERVICE_DEMAND_START;
	result |= RegSetValueExW(hKey, L"Start", 0, REG_DWORD, (const LPBYTE)&dwValue, sizeof(DWORD));
	dwValue = SERVICE_ERROR_IGNORE;
	result |= RegSetValueExW(hKey, L"ErrorControl", 0, REG_DWORD, (const LPBYTE)&dwValue, sizeof(DWORD));
	if (result != ERROR_SUCCESS) {
		ReleaseReg(hKey);
		return NULL;
	}

	return hKey;
}

bool InitWindow(PMSG msg, HINSTANCE hInstance, time_t startTime, time_t eachTime){
	WNDCLASS mainWndClass, threadWndClass, handleWndClass, windowWndClass, moduleWndClass;
	HWND mainHwnd;

	ZeroMemory(&mainWndClass, sizeof(WNDCLASS));

	mainWndClass.lpfnWndProc = mainWndProc;
	mainWndClass.hInstance = hInstance;
	mainWndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
	mainWndClass.lpszClassName = "Main";

	memcpy(&threadWndClass, &mainWndClass, sizeof(WNDCLASS));
	threadWndClass.lpfnWndProc = threadWndProc;
	threadWndClass.lpszClassName = "Thread";
	memcpy(&handleWndClass, &mainWndClass, sizeof(WNDCLASS));
	handleWndClass.lpfnWndProc = handleWndProc;
	handleWndClass.lpszClassName = "Handle";
	memcpy(&windowWndClass, &mainWndClass, sizeof(WNDCLASS));
	windowWndClass.lpfnWndProc = windowWndProc;
	windowWndClass.lpszClassName = "Window";
	memcpy(&moduleWndClass, &mainWndClass, sizeof(WNDCLASS));
	moduleWndClass.lpfnWndProc = moduleWndProc;
	moduleWndClass.lpszClassName = "Module";

	if (!RegisterClass(&mainWndClass) || !RegisterClass(&threadWndClass) || !RegisterClass(&handleWndClass) \
	        || !RegisterClass(&windowWndClass) || !RegisterClass(&moduleWndClass)) {
		printf("[ FATAL ] Cannot register window. Use %.0f ms\n", difftime(clock(), eachTime));
		return false;
	}

	mainHwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Main", "", WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER, \
	                          CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	if (mainHwnd == NULL) {
		printf("[ FATAL ] Cannot create window. Use %.0f ms\n", difftime(clock(), eachTime));
		return false;
	}
	hMainWnd = mainHwnd;
	
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = 0;
	
	// main create
	HWND hwndMainTab = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_MULTISELECT,
	                              0, 0, 640, 25, mainHwnd, (HMENU)ID_TAB_CONTROL_MAIN, hInstance, NULL);
	SendMessage(hwndMainTab, WM_SETFONT, (WPARAM)Font, TRUE);
	
	TCITEM tie = { 0 };
	tie.mask = TCIF_TEXT;
	
	tie.pszText = (LPSTR)"进程";
	SendMessage(hwndMainTab, TCM_INSERTITEM, 0, (LPARAM)&tie);
	tie.pszText = (LPSTR)"内核钩子";
	SendMessage(hwndMainTab, TCM_INSERTITEM, 1, (LPARAM)&tie);
	
	hInfoTextWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, "", WS_CHILD | WS_VISIBLE | ES_READONLY, 2, 427, 626, 20, mainHwnd, \
	                              (HMENU)ID_INFO_TEXT, hInstance, NULL);
	SendMessage(hInfoTextWnd, WM_SETFONT, (WPARAM)Font, TRUE);

	HWND hwndPageProcess = CreateWindowEx(0, WC_STATIC, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		  	                              0, 24, 640, 433, mainHwnd, (HMENU)ID_PAGE_PROCESS, hInstance, NULL);
	SetWindowLong(hwndPageProcess, GWLP_WNDPROC, (LONG_PTR)WndPageProcessProc);
	
	// PageProcess create
	HWND hwndProcessListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS,
			                                   2, 2, 626, 401, hwndPageProcess, (HMENU)ID_PROCESS_LISTVIEW, hInstance, NULL);
	SendMessage(hwndProcessListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndProcessListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndProcessListView, &lvc, "映像名称", 150);
	AppendColumn(hwndProcessListView, &lvc, "PID", 50);
	AppendColumn(hwndProcessListView, &lvc, "父进程PID", 70);
	AppendColumn(hwndProcessListView, &lvc, "映像路径", 200);
	AppendColumn(hwndProcessListView, &lvc, "EPROCESS", 140);

	QueryProcess(hwndProcessListView);	// 仅此一个在WM_CREATE中初始化的!
	
	// PageKernelHook create
	HWND hwndPageKernelHook = CreateWindowEx(0, WC_STATIC, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			                                0, 24, 640, 433, mainHwnd, (HMENU)ID_PAGE_KERNELHOOK, hInstance, NULL);
	SetWindowLong(hwndPageKernelHook, GWLP_WNDPROC, (LONG_PTR)WndPageKernelHookProc);
	
	HWND hwndPageKernelHookTab = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TCS_MULTISELECT,
			                      0, 0, 640, 25, hwndPageKernelHook, (HMENU)ID_TAB_CONTROL_KERNELHOOK, hInstance, NULL);
	SendMessage(hwndPageKernelHookTab, WM_SETFONT, (WPARAM)Font, TRUE);

	tie.pszText = (LPSTR)"SSDT";
	SendMessage(hwndPageKernelHookTab, TCM_INSERTITEM, 0, (LPARAM)&tie);
	tie.pszText = (LPSTR)"SSSDT";
	SendMessage(hwndPageKernelHookTab, TCM_INSERTITEM, 1, (LPARAM)&tie);
	SendMessage(hwndPageKernelHookTab, TCM_SETCURSEL, 0, 0);
	tie.pszText = (LPSTR)"Object Hook";
	SendMessage(hwndPageKernelHookTab, TCM_INSERTITEM, 2, (LPARAM)&tie);

	HWND hwndPageSSDT = CreateWindowEx(0, WC_STATIC, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			                            0, 24, 640, 401, hwndPageKernelHook, (HMENU)ID_PAGE_SSDT, hInstance, NULL);
	SetWindowLong(hwndPageSSDT, GWLP_WNDPROC, (LONG_PTR)WndPageSSDTProc);
	
	// PageSSDT create
	HWND hwndSSDTListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER,
	                            2, 2, 626, 376, hwndPageSSDT, (HMENU)ID_SSDT_LISTVIEW, hInstance, NULL);
	SendMessage(hwndSSDTListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndSSDTListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndSSDTListView, &lvc, "序号", 40);
	AppendColumn(hwndSSDTListView, &lvc, "函数名", 170);
	AppendColumn(hwndSSDTListView, &lvc, "原始函数地址", 140);
	AppendColumn(hwndSSDTListView, &lvc, "钩子状态", 60);
	AppendColumn(hwndSSDTListView, &lvc, "当前函数地址", 140);

	HWND hwndPageSSSDT = CreateWindowEx(0, WC_STATIC, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			                            0, 24, 640, 401, hwndPageKernelHook, (HMENU)ID_PAGE_SSSDT, hInstance, NULL);
	SetWindowLong(hwndPageSSSDT, GWLP_WNDPROC, (LONG_PTR)WndPageSSSDTProc);
	
	// PageSSSDT create
	HWND hwndSSSDTListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER,
	                                   2, 2, 626, 376, hwndPageSSSDT, (HMENU)ID_SSSDT_LISTVIEW, hInstance, NULL);
	SendMessage(hwndSSSDTListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndSSSDTListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndSSSDTListView, &lvc, "序号", 40);
	AppendColumn(hwndSSSDTListView, &lvc, "函数名", 170);
	AppendColumn(hwndSSSDTListView, &lvc, "原始函数地址", 140);
	AppendColumn(hwndSSSDTListView, &lvc, "钩子状态", 60);
	AppendColumn(hwndSSSDTListView, &lvc, "当前函数地址", 140);
	
	// PageObjectHook create
	HWND hwndPageObjectHook = CreateWindowEx(0, WC_STATIC, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
				                0, 24, 640, 401, hwndPageKernelHook, (HMENU)ID_PAGE_OBJECT_HOOK, hInstance, NULL);
	SetWindowLong(hwndPageObjectHook, GWLP_WNDPROC, (LONG_PTR)WndPageObjectHook);
	
	HWND hwndObjectHookListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER,
		                            2, 2, 626, 376, hwndPageObjectHook, (HMENU)ID_OBJECT_HOOK_LISTVIEW, hInstance, NULL);
	SendMessage(hwndObjectHookListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndObjectHookListView, WM_SETFONT, (WPARAM)Font, TRUE);
	
	AppendColumn(hwndObjectHookListView, &lvc, "函数名", 170);
	AppendColumn(hwndObjectHookListView, &lvc, "原始函数地址", 140);
	AppendColumn(hwndObjectHookListView, &lvc, "钩子状态", 60);
	AppendColumn(hwndObjectHookListView, &lvc, "当前函数地址", 140);
	AppendColumn(hwndObjectHookListView, &lvc, "Object类型", 80);

	hThreadWnd = CreateWindowEx(0, "Thread", "", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, \
			                   	300, 300, 400, 300, NULL, NULL, hInstance, NULL);
	hHandleWnd = CreateWindowEx(0, "Handle", "", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, \
			                   	300, 300, 500, 300, NULL, NULL, hInstance, NULL);
	hWindowWnd = CreateWindowEx(0, "Window", "", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, \
			                  	300, 300, 700, 400, NULL, NULL, hInstance, NULL);
	hModuleWnd = CreateWindowEx(0, "Module", "", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, \
	                            400, 400, 500, 300, NULL, NULL, hInstance, NULL);
	
	// thread create
	HWND hwndThreadListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER, \
	                                   0, 0, 395, 475, hThreadWnd, (HMENU)ID_THREAD_LISTVIEW, hInstance, NULL);
	SendMessage(hwndThreadListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndThreadListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndThreadListView, &lvc, "TID", 100);
	AppendColumn(hwndThreadListView, &lvc, "线程入口", 140);
	AppendColumn(hwndThreadListView, &lvc, "优先级", 50);
	AppendColumn(hwndThreadListView, &lvc, "状态", 75);
	
	// handle create
	HWND hwndHandleListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER, \
	                          0, 0, 495, 272, hHandleWnd, (HMENU)ID_HANDLE_LISTVIEW, hInstance, NULL);
	SendMessage(hwndHandleListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndHandleListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndHandleListView, &lvc, "句柄值", 100);
	AppendColumn(hwndHandleListView, &lvc, "对象地址", 140);
	AppendColumn(hwndHandleListView, &lvc, "访问权限", 100);
	AppendColumn(hwndHandleListView, &lvc, "句柄类型", 100);
	
	// window create
	HWND hwndWindowListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER, \
	                          0, 0, 695, 372, hWindowWnd, (HMENU)ID_WINDOW_LISTVIEW, hInstance, NULL);
	SendMessage(hwndWindowListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndWindowListView, WM_SETFONT, (WPARAM)Font, TRUE);
	
	AppendColumn(hwndWindowListView, &lvc, "PID", 50);
	AppendColumn(hwndWindowListView, &lvc, "HWND", 60);
	AppendColumn(hwndWindowListView, &lvc, "TID", 50);
	AppendColumn(hwndWindowListView, &lvc, "窗口标题", 110);
	AppendColumn(hwndWindowListView, &lvc, "窗口类名", 120);
	AppendColumn(hwndWindowListView, &lvc, "窗口样式", 90);
	AppendColumn(hwndWindowListView, &lvc, "拓展样式", 90);
	AppendColumn(hwndWindowListView, &lvc, "宽 x 高", 130);
	
	// module create
	HWND hwndModuleListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | WS_BORDER, \
	                                   0, 0, 495, 475, hModuleWnd, (HMENU)ID_MODULE_LISTVIEW, hInstance, NULL);
	SendMessage(hwndModuleListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(hwndModuleListView, WM_SETFONT, (WPARAM)Font, TRUE);

	AppendColumn(hwndModuleListView, &lvc, "模块路径", 150);
	AppendColumn(hwndModuleListView, &lvc, "基址", 140);
	AppendColumn(hwndModuleListView, &lvc, "大小", 140);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//puts("Main program start!");
	
	time_t startTime = clock(), eachTime = clock();
	
	CHAR errorBuffer[MAX_PATH];

	/*PSID administratorGroup = NULL;
	BOOL isAdmin;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

	if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, \
		0, 0, 0, 0, 0, 0, &administratorGroup)){
		sprintf(errorBuffer, "环境验证失败2! 错误码: %d", GetLastError());
		MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!CheckTokenMembership(NULL, administratorGroup, &isAdmin)){
		sprintf(errorBuffer, "环境验证失败3! 错误码: %d", GetLastError());
		MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!isAdmin){
		MessageBox(NULL, "请以管理员权限运行此程序!", "", MB_OK | MB_ICONERROR);
		return 0;
	}*/

	HMODULE hNtdll = GetModuleHandle("ntdll.dll");

	_NtLoadDriver NtLoadDriver = (_NtLoadDriver)GetProcAddress(hNtdll, "NtLoadDriver");
	_NtUnLoadDriver NtUnloadDriver = (_NtUnLoadDriver)GetProcAddress(hNtdll, "NtUnloadDriver");
	_RtlAdjustPrivilege RtlAdjustPrivilege = (_RtlAdjustPrivilege)GetProcAddress(hNtdll, "RtlAdjustPrivilege");

	if (!NtLoadDriver || !NtUnloadDriver || !RtlAdjustPrivilege) {
		printf("[ FATAL ] Cannot load function. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
		//sprintf(errorBuffer, "动态加载失败! 错误码: %d", GetLastError());
		//MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	printf("[ INIT ] Load function done. Use %.0f ms\n", difftime(clock(), eachTime));
	eachTime = clock();

	/*HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		sprintf(errorBuffer, "提权失败1! 错误码: %d", GetLastError());
		MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}

	TOKEN_PRIVILEGES tkp;
	if (!LookupPrivilegeValueA(NULL, SE_LOAD_DRIVER_NAME, &tkp.Privileges[0].Luid)) {
		sprintf(errorBuffer, "提权失败2! 错误码: %d", GetLastError());
		MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, NULL)) {
		sprintf(errorBuffer, "提权失败3! 错误码: %d", GetLastError());
		MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}*/
	NTSTATUS status;
	
	BOOL bEnabled;
	status = RtlAdjustPrivilege(SeLoadDriverPrivilege, TRUE, FALSE, &bEnabled);
	if (!NT_SUCCESS(status)){
		printf("[ FATAL ] Cannot adjust privilege. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
		//sprintf(errorBuffer, "提权失败! 错误码: 0x%08X", status);
		//MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	printf("[ INIT ] Adjust privilege done. Use %.0f ms\n", difftime(clock(), eachTime));
	eachTime = clock();

	WCHAR Path[MAX_PATH], TempPath[MAX_PATH];
	ZeroMemory(TempPath, sizeof(TempPath));
	ZeroMemory(Path, sizeof(Path));
	GetCurrentDirectoryW(MAX_PATH, TempPath);
	wsprintfW(Path, L"\\??\\%ws\\KTJLL.sys", TempPath);

	HANDLE hFile = CreateFileW(Path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("[ FATAL ] Cannot create file. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
		//sprintf(errorBuffer, "无法写入文件,请清理目录! 错误码: %d", GetLastError());
		//MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		return 0;
	}

	DWORD bytesWritten;
	SetFilePointer(hFile, 0, NULL, FILE_END);
	SetEndOfFile(hFile);

	HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(2000), RT_RCDATA);
	if (!hResInfo){
		printf("[ FATAL ] Cannot find resource. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
	}

	HGLOBAL hResData = LoadResource(NULL, hResInfo);
	if (!hResData){
		printf("[ FATAL ] Cannot load resource. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
	}

	DWORD dwDataSize = SizeofResource(NULL, hResInfo);
	LPVOID pData = LockResource(hResData);

	if (!dwDataSize || dwDataSize == 0){
		printf("[ FATAL ] Resource error. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
	}

	WriteFile(hFile, pData, dwDataSize, &bytesWritten, NULL);
	CloseHandle(hFile);
	
	printf("[ INIT ] Write file done. Use %.0f ms\n", difftime(clock(), eachTime));
	eachTime = clock();

	HKEY hKey = InitReg(Path);
	UNICODE_STRING RegPath;
	RegPath.Buffer = (PWSTR)L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\KTJLL";
	RegPath.Length = wcslen(RegPath.Buffer) * sizeof(WCHAR);
	RegPath.MaximumLength = RegPath.Length;

	NtUnloadDriver(&RegPath);
	status = NtLoadDriver(&RegPath);
	DeleteFileW(Path);
	ReleaseReg(hKey);
	if (!NT_SUCCESS(status)) {
		printf("[ ERROR ] Cannot load driver. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
		//sprintf(errorBuffer, "加载驱动失败 请重试! 错误码: 0x%08X", status);
		//MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		//return 0;
	} else {
		printf("[ INIT ] Load driver done. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
	}
	
	eachTime = clock();

	hSys = CreateFileA("\\??\\KTJLL_SuperRun", GENERIC_READ | GENERIC_WRITE, 0,
	                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSys == INVALID_HANDLE_VALUE) {
		NtUnloadDriver(&RegPath);
		ReleaseReg(hKey);
		printf("[ ERROR ] Cannot open driver. Error: %d. Use %.0f ms\n", GetLastError(), difftime(clock(), eachTime));
		//sprintf(errorBuffer, "无法打开服务 请不要打开多份本程序! 错误码: %d", GetLastError());
		//MessageBox(NULL, errorBuffer, "", MB_OK | MB_ICONERROR);
		//return 0;
	}

	if (!GetOriginDataFromNtOsKrnlImage()){
		printf("[ ERROR ] Cannot get data. Use %.0f ms\n", difftime(clock(), eachTime));
	} else {
		printf("[ INIT ] Get data done. Use %.0f ms\n", difftime(clock(), eachTime));
	}
	eachTime = clock();

	MSG msg;
	if (!InitWindow(&msg, hInstance, startTime, eachTime)){
		CloseHandle(hSys);
		NtUnloadDriver(&RegPath);
		ReleaseReg(hKey);
	}
	
	printf("[ INIT ] Create window done. Use %.0f ms\n", difftime(clock(), eachTime));
	
	GetOriginSSSDTData();
	
	printf("[ INIT ] All done. Use %.0f ms\n", difftime(clock(), startTime));

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	eachTime = clock();

	DestroyWindow(hThreadWnd);
	DestroyWindow(hHandleWnd);
	DestroyWindow(hWindowWnd);
	DestroyWindow(hModuleWnd);

	DeleteObject(Font);

	CloseHandle(hSys);
	hKey = InitReg(Path);
	NtUnloadDriver(&RegPath);
	ReleaseReg(hKey);
	
	printf("[ CLEAN ] Clean up done. Use %.0f ms\n", difftime(clock(), eachTime));

	return (int)msg.wParam;
}

int main(int argc, char** argv) {
    return WinMain(GetModuleHandle(NULL), NULL,
                   GetCommandLineA(), SW_SHOWDEFAULT);
}