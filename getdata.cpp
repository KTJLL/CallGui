#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#include "defines.h"
#include "main.h"

ULONG64 OriginSSDTData[SSDT_NUMBERS];
const char* ssdtfunc[SSDT_NUMBERS] = {
    "MapUserPhysicalPagesScatter", 
    "WaitForSingleObject", 
    "CallbackReturn", 
    "ReadFile", 
    "DeviceIoControlFile", 
    "WriteFile", 
    "RemoveIoCompletion", 
    "ReleaseSemaphore", 
    "ReplyWaitReceivePort", 
    "ReplyPort", 
    "SetInformationThread", 
    "SetEvent", 
    "Close", 
    "QueryObject", 
    "QueryInformationFile", 
    "OpenKey", 
    "EnumerateValueKey", 
    "FindAtom", 
    "QueryDefaultLocale", 
    "QueryKey", 
    "QueryValueKey", 
    "AllocateVirtualMemory", 
    "QueryInformationProcess", 
    "WaitForMultipleObjects32", 
    "WriteFileGather", 
    "SetInformationProcess", 
    "CreateKey", 
    "FreeVirtualMemory", 
    "ImpersonateClientOfPort", 
    "ReleaseMutant", 
    "QueryInformationToken", 
    "RequestWaitReplyPort", 
    "QueryVirtualMemory", 
    "OpenThreadToken", 
    "QueryInformationThread", 
    "OpenProcess", 
    "SetInformationFile", 
    "MapViewOfSection", 
    "AccessCheckAndAuditAlarm", 
    "UnmapViewOfSection", 
    "ReplyWaitReceivePortEx", 
    "TerminateProcess", 
    "SetEventBoostPriority", 
    "ReadFileScatter", 
    "OpenThreadTokenEx", 
    "OpenProcessTokenEx", 
    "QueryPerformanceCounter", 
    "EnumerateKey", 
    "OpenFile", 
    "DelayExecution", 
    "QueryDirectoryFile", 
    "QuerySystemInformation", 
    "OpenSection", 
    "QueryTimer", 
    "FsControlFile", 
    "WriteVirtualMemory", 
    "CloseObjectAuditAlarm", 
    "DuplicateObject", 
    "QueryAttributesFile", 
    "ClearEvent", 
    "ReadVirtualMemory", 
    "OpenEvent", 
    "AdjustPrivilegesToken", 
    "DuplicateToken", 
    "Continue", 
    "QueryDefaultUILanguage", 
    "QueueApcThread", 
    "YieldExecution", 
    "AddAtom", 
    "CreateEvent", 
    "QueryVolumeInformationFile", 
    "CreateSection", 
    "FlushBuffersFile", 
    "ApphelpCacheControl", 
    "CreateProcessEx", 
    "CreateThread", 
    "IsProcessInJob", 
    "ProtectVirtualMemory", 
    "QuerySection", 
    "ResumeThread", 
    "TerminateThread", 
    "ReadRequestData", 
    "CreateFile", 
    "QueryEvent", 
    "WriteRequestData", 
    "OpenDirectoryObject", 
    "AccessCheckByTypeAndAuditAlarm", 
    "QuerySystemTime", 
    "WaitForMultipleObjects", 
    "SetInformationObject", 
    "CancelIoFile", 
    "TraceEvent", 
    "PowerInformation", 
    "SetValueKey", 
    "CancelTimer", 
    "SetTimer", 
    "AcceptConnectPort", 
    "AccessCheck", 
    "AccessCheckByType", 
    "AccessCheckByTypeResultList", 
    "AccessCheckByTypeResultListAndAuditAlarm", 
    "AccessCheckByTypeResultListAndAuditAlarmByHandle", 
    "AddBootEntry", 
    "AddDriverEntry", 
    "AdjustGroupsToken", 
    "AlertResumeThread", 
    "AlertThread", 
    "AllocateLocallyUniqueId", 
    "AllocateReserveObject", 
    "AllocateUserPhysicalPages", 
    "AllocateUuids", 
    "AlpcAcceptConnectPort", 
    "AlpcCancelMessage", 
    "AlpcConnectPort", 
    "AlpcCreatePort", 
    "AlpcCreatePortSection", 
    "AlpcCreateResourceReserve", 
    "AlpcCreateSectionView", 
    "AlpcCreateSecurityContext", 
    "AlpcDeletePortSection", 
    "AlpcDeleteResourceReserve", 
    "AlpcDeleteSectionView", 
    "AlpcDeleteSecurityContext", 
    "AlpcDisconnectPort", 
    "AlpcImpersonateClientOfPort", 
    "AlpcOpenSenderProcess", 
    "AlpcOpenSenderThread", 
    "AlpcQueryInformation", 
    "AlpcQueryInformationMessage", 
    "AlpcRevokeSecurityContext", 
    "AlpcSendWaitReceivePort", 
    "AlpcSetInformation", 
    "AreMappedFilesTheSame", 
    "AssignProcessToJobObject", 
    "CancelIoFileEx", 
    "CancelSynchronousIoFile", 
    "CommitComplete", 
    "CommitEnlistment", 
    "CommitTransaction", 
    "CompactKeys", 
    "CompareTokens", 
    "CompleteConnectPort", 
    "CompressKey", 
    "ConnectPort", 
    "CreateDebugObject", 
    "CreateDirectoryObject", 
    "CreateEnlistment", 
    "CreateEventPair", 
    "CreateIoCompletion", 
    "CreateJobObject", 
    "CreateJobSet", 
    "CreateKeyTransacted", 
    "CreateKeyedEvent", 
    "CreateMailslotFile", 
    "CreateMutant", 
    "CreateNamedPipeFile", 
    "CreatePagingFile", 
    "CreatePort", 
    "CreatePrivateNamespace", 
    "CreateProcess", 
    "CreateProfile", 
    "CreateProfileEx", 
    "CreateResourceManager", 
    "CreateSemaphore", 
    "CreateSymbolicLinkObject", 
    "CreateThreadEx", 
    "CreateTimer", 
    "CreateToken", 
    "CreateTransaction", 
    "CreateTransactionManager", 
    "CreateUserProcess", 
    "CreateWaitablePort", 
    "CreateWorkerFactory", 
    "DebugActiveProcess", 
    "DebugContinue", 
    "DeleteAtom", 
    "DeleteBootEntry", 
    "DeleteDriverEntry", 
    "DeleteFile", 
    "DeleteKey", 
    "DeleteObjectAuditAlarm", 
    "DeletePrivateNamespace", 
    "DeleteValueKey", 
    "DisableLastKnownGood", 
    "DisplayString", 
    "DrawText", 
    "EnableLastKnownGood", 
    "EnumerateBootEntries", 
    "EnumerateDriverEntries", 
    "EnumerateSystemEnvironmentValuesEx", 
    "EnumerateTransactionObject", 
    "ExtendSection", 
    "FilterToken", 
    "FlushInstallUILanguage", 
    "FlushInstructionCache", 
    "FlushKey", 
    "FlushProcessWriteBuffers", 
    "FlushVirtualMemory", 
    "FlushWriteBuffer", 
    "FreeUserPhysicalPages", 
    "FreezeRegistry", 
    "FreezeTransactions", 
    "GetContextThread", 
    "GetCurrentProcessorNumber", 
    "GetDevicePowerState", 
    "GetMUIRegistryInfo", 
    "GetNextProcess", 
    "GetNextThread", 
    "GetNlsSectionPtr", 
    "GetNotificationResourceManager", 
    "GetPlugPlayEvent", 
    "GetWriteWatch", 
    "ImpersonateAnonymousToken", 
    "ImpersonateThread", 
    "InitializeNlsFiles", 
    "InitializeRegistry", 
    "InitiatePowerAction", 
    "IsSystemResumeAutomatic", 
    "IsUILanguageComitted", 
    "ListenPort", 
    "LoadDriver", 
    "LoadKey", 
    "LoadKey2", 
    "LoadKeyEx", 
    "LockFile", 
    "LockProductActivationKeys", 
    "LockRegistryKey", 
    "LockVirtualMemory", 
    "MakePermanentObject", 
    "MakeTemporaryObject", 
    "MapCMFModule", 
    "MapUserPhysicalPages", 
    "ModifyBootEntry", 
    "ModifyDriverEntry", 
    "NotifyChangeDirectoryFile", 
    "NotifyChangeKey", 
    "NotifyChangeMultipleKeys", 
    "NotifyChangeSession", 
    "OpenEnlistment", 
    "OpenEventPair", 
    "OpenIoCompletion", 
    "OpenJobObject", 
    "OpenKeyEx", 
    "OpenKeyTransacted", 
    "OpenKeyTransactedEx", 
    "OpenKeyedEvent", 
    "OpenMutant", 
    "OpenObjectAuditAlarm", 
    "OpenPrivateNamespace", 
    "OpenProcessToken", 
    "OpenResourceManager", 
    "OpenSemaphore", 
    "OpenSession", 
    "OpenSymbolicLinkObject", 
    "OpenThread", 
    "OpenTimer", 
    "OpenTransaction", 
    "OpenTransactionManager", 
    "PlugPlayControl", 
    "PrePrepareComplete", 
    "PrePrepareEnlistment", 
    "PrepareComplete", 
    "PrepareEnlistment", 
    "PrivilegeCheck", 
    "PrivilegeObjectAuditAlarm", 
    "PrivilegedServiceAuditAlarm", 
    "PropagationComplete", 
    "PropagationFailed", 
    "PulseEvent", 
    "QueryBootEntryOrder", 
    "QueryBootOptions", 
    "QueryDebugFilterState", 
    "QueryDirectoryObject", 
    "QueryDriverEntryOrder", 
    "QueryEaFile", 
    "QueryFullAttributesFile", 
    "QueryInformationAtom", 
    "QueryInformationEnlistment", 
    "QueryInformationJobObject", 
    "QueryInformationPort", 
    "QueryInformationResourceManager", 
    "QueryInformationTransaction", 
    "QueryInformationTransactionManager", 
    "QueryInformationWorkerFactory", 
    "QueryInstallUILanguage", 
    "QueryIntervalProfile", 
    "QueryIoCompletion", 
    "QueryLicenseValue", 
    "QueryMultipleValueKey", 
    "QueryMutant", 
    "QueryOpenSubKeys", 
    "QueryOpenSubKeysEx", 
    "QueryPortInformationProcess", 
    "QueryQuotaInformationFile", 
    "QuerySecurityAttributesToken", 
    "QuerySecurityObject", 
    "QuerySemaphore", 
    "QuerySymbolicLinkObject", 
    "QuerySystemEnvironmentValue", 
    "QuerySystemEnvironmentValueEx", 
    "QuerySystemInformationEx", 
    "QueryTimerResolution", 
    "QueueApcThreadEx", 
    "RaiseException", 
    "RaiseHardError", 
    "ReadOnlyEnlistment", 
    "RecoverEnlistment", 
    "RecoverResourceManager", 
    "RecoverTransactionManager", 
    "RegisterProtocolAddressInformation", 
    "RegisterThreadTerminatePort", 
    "ReleaseKeyedEvent", 
    "ReleaseWorkerFactoryWorker", 
    "RemoveIoCompletionEx", 
    "RemoveProcessDebug", 
    "RenameKey", 
    "RenameTransactionManager", 
    "ReplaceKey", 
    "ReplacePartitionUnit", 
    "ReplyWaitReplyPort", 
    "RequestPort", 
    "ResetEvent", 
    "ResetWriteWatch", 
    "RestoreKey", 
    "ResumeProcess", 
    "RollbackComplete", 
    "RollbackEnlistment", 
    "RollbackTransaction", 
    "RollforwardTransactionManager", 
    "SaveKey", 
    "SaveKeyEx", 
    "SaveMergedKeys", 
    "SecureConnectPort", 
    "SerializeBoot", 
    "SetBootEntryOrder", 
    "SetBootOptions", 
    "SetContextThread", 
    "SetDebugFilterState", 
    "SetDefaultHardErrorPort", 
    "SetDefaultLocale", 
    "SetDefaultUILanguage", 
    "SetDriverEntryOrder", 
    "SetEaFile", 
    "SetHighEventPair", 
    "SetHighWaitLowEventPair", 
    "SetInformationDebugObject", 
    "SetInformationEnlistment", 
    "SetInformationJobObject", 
    "SetInformationKey", 
    "SetInformationResourceManager", 
    "SetInformationToken", 
    "SetInformationTransaction", 
    "SetInformationTransactionManager", 
    "SetInformationWorkerFactory", 
    "SetIntervalProfile", 
    "SetIoCompletion", 
    "SetIoCompletionEx", 
    "SetLdtEntries", 
    "SetLowEventPair", 
    "SetLowWaitHighEventPair", 
    "SetQuotaInformationFile", 
    "SetSecurityObject", 
    "SetSystemEnvironmentValue", 
    "SetSystemEnvironmentValueEx", 
    "SetSystemInformation", 
    "SetSystemPowerState", 
    "SetSystemTime", 
    "SetThreadExecutionState", 
    "SetTimerEx", 
    "SetTimerResolution", 
    "SetUuidSeed", 
    "SetVolumeInformationFile", 
    "ShutdownSystem", 
    "ShutdownWorkerFactory", 
    "SignalAndWaitForSingleObject", 
    "SinglePhaseReject", 
    "StartProfile", 
    "StopProfile", 
    "SuspendProcess", 
    "SuspendThread", 
    "SystemDebugControl", 
    "TerminateJobObject", 
    "TestAlert", 
    "ThawRegistry", 
    "ThawTransactions", 
    "TraceControl", 
    "TranslateFilePath", 
    "UmsThreadYield", 
    "UnloadDriver", 
    "UnloadKey", 
    "UnloadKey2", 
    "UnloadKeyEx", 
    "UnlockFile", 
    "UnlockVirtualMemory", 
    "VdmControl", 
    "WaitForDebugEvent", 
    "WaitForKeyedEvent", 
    "WaitForWorkViaWorkerFactory", 
    "WaitHighEventPair", 
    "WaitLowEventPair", 
    "WorkerFactoryWorkerReady"
};

ULONG64 OriginSSSDTData[SSSDT_NUMBERS];
const char* sssdtfunc[SSSDT_NUMBERS] = {
    "UserGetThreadState",  
    "UserPeekMessage",  
    "UserCallOneParam",  
    "UserGetKeyState",  
    "UserInvalidateRect",  
    "UserCallNoParam",  
    "UserGetMessage",  
    "UserMessageCall",  
    "GdiBitBlt",  
    "GdiGetCharSet",  
    "UserGetDC",  
    "GdiSelectBitmap",  
    "UserWaitMessage",  
    "UserTranslateMessage",  
    "UserGetProp",  
    "UserPostMessage",  
    "UserQueryWindow",  
    "UserTranslateAccelerator",  
    "GdiFlush",  
    "UserRedrawWindow",  
    "UserWindowFromPoi",  
    "UserCallMsgFilter",  
    "UserValidateTimerCallback",  
    "UserBeginPai",  
    "UserSetTimer",  
    "UserEndPai",  
    "UserSetCursor",  
    "UserKillTimer",  
    "UserBuildHwndList",  
    "UserSelectPalette",  
    "UserCallNextHookEx",  
    "UserHideCaret",  
    "GdiIersectClipRect",  
    "UserCallHwndLock",  
    "UserGetProcessWindowStation",  
    "GdiDeleteObjectApp",  
    "UserSetWindowPos",  
    "UserShowCaret",  
    "UserEndDeferWindowPosEx",  
    "UserCallHwndParamLock",  
    "UserVkKeyScanEx",  
    "GdiSetDIBitsToDeviceIernal",  
    "UserCallTwoParam",  
    "GdiGetRandomRgn",  
    "UserCopyAcceleratorTable",  
    "UserNotifyWinEve",  
    "GdiExtSelectClipRgn",  
    "UserIsClipboardFormatAvailable",  
    "UserSetScrollInfo",  
    "GdiStretchBlt",  
    "UserCreateCaret",  
    "GdiRectVisible",  
    "GdiCombineRgn",  
    "GdiGetDCObject",  
    "UserDispatchMessage",  
    "UserRegisterWindowMessage",  
    "GdiExtTextOutW",  
    "GdiSelectFo",  
    "GdiRestoreDC",  
    "GdiSaveDC",  
    "UserGetForegroundWindow",  
    "UserShowScrollBar",  
    "UserFindExistingCursorIcon",  
    "GdiGetDCDword",  
    "GdiGetRegionData",  
    "GdiLineTo",  
    "UserSystemParametersInfo",  
    "GdiGetAppClipBox",  
    "UserGetAsyncKeyState",  
    "UserGetCPD",  
    "UserRemoveProp",  
    "GdiDoPalette",  
    "GdiPolyPolyDraw",  
    "UserSetCapture",  
    "UserEnumDisplayMonitors",  
    "GdiCreateCompatibleBitmap",  
    "UserSetProp",  
    "GdiGetTextCharsetInfo",  
    "UserSBGetParms",  
    "UserGetIconInfo",  
    "UserExcludeUpdateRgn",  
    "UserSetFocus",  
    "GdiExtGetObjectW",  
    "UserDeferWindowPos",  
    "UserGetUpdateRect",  
    "GdiCreateCompatibleDC",  
    "UserGetClipboardSequenceNumber",  
    "GdiCreatePen",  
    "UserShowWindow",  
    "UserGetKeyboardLayoutList",  
    "GdiPatBlt",  
    "UserMapVirtualKeyEx",  
    "UserSetWindowLong",  
    "GdiHfoCreate",  
    "UserMoveWindow",  
    "UserPostThreadMessage",  
    "UserDrawIconEx",  
    "UserGetSystemMenu",  
    "GdiDrawStream",  
    "UserIernalGetWindowText",  
    "UserGetWindowDC",  
    "GdiD3dDrawPrimitives2",  
    "GdiInvertRgn",  
    "GdiGetRgnBox",  
    "GdiGetAndSetDCDword",  
    "GdiMaskBlt",  
    "GdiGetWidthTable",  
    "UserScrollDC",  
    "UserGetObjectInformation",  
    "GdiCreateBitmap",  
    "UserFindWindowEx",  
    "GdiPolyPatBlt",  
    "UserUnhookWindowsHookEx",  
    "GdiGetNearestColor",  
    "GdiTransformPois",  
    "GdiGetDCPoi",  
    "GdiCreateDIBBrush",  
    "GdiGetTextMetricsW",  
    "UserCreateWindowEx",  
    "UserSetPare",  
    "UserGetKeyboardState",  
    "UserToUnicodeEx",  
    "UserGetCorolBrush",  
    "UserGetClassName",  
    "GdiAlphaBlend",  
    "GdiDdBlt",  
    "GdiOffsetRgn",  
    "UserDefSetText",  
    "GdiGetTextFaceW",  
    "GdiStretchDIBitsIernal",  
    "UserSendInput",  
    "UserGetThreadDesktop",  
    "GdiCreateRectRgn",  
    "GdiGetDIBitsIernal",  
    "UserGetUpdateRgn",  
    "GdiDeleteClieObj",  
    "UserGetIconSize",  
    "UserFillWindow",  
    "GdiExtCreateRegion",  
    "GdiComputeXformCoefficies",  
    "UserSetWindowsHookEx",  
    "UserNotifyProcessCreate",  
    "GdiUnrealizeObject",  
    "UserGetTitleBarInfo",  
    "GdiRectangle",  
    "UserSetThreadDesktop",  
    "UserGetDCEx",  
    "UserGetScrollBarInfo",  
    "GdiGetTextExte",  
    "UserSetWindowFNID",  
    "GdiSetLayout",  
    "UserCalcMenuBar",  
    "UserThunkedMenuItemInfo",  
    "GdiExcludeClipRect",  
    "GdiCreateDIBSection",  
    "GdiGetDCforBitmap",  
    "UserDestroyCursor",  
    "UserDestroyWindow",  
    "UserCallHwndParam",  
    "GdiCreateDIBitmapIernal",  
    "UserOpenWindowStation",  
    "GdiDdDeleteSurfaceObject",  
    "GdiDdCanCreateSurface",  
    "GdiDdCreateSurface",  
    "UserSetCursorIconData",  
    "GdiDdDestroySurface",  
    "UserCloseDesktop",  
    "UserOpenDesktop",  
    "UserSetProcessWindowStation",  
    "UserGetAtomName",  
    "GdiDdResetVisrgn",  
    "GdiExtCreatePen",  
    "GdiCreatePaletteIernal",  
    "GdiSetBrushOrg",  
    "UserBuildNameList",  
    "GdiSetPixel",  
    "UserRegisterClassExWOW",  
    "GdiCreatePatternBrushIernal",  
    "UserGetAncestor",  
    "GdiGetOutlineTextMetricsIernalW",  
    "GdiSetBitmapBits",  
    "UserCloseWindowStation",  
    "UserGetDoubleClickTime",  
    "UserEnableScrollBar",  
    "GdiCreateSolidBrush",  
    "UserGetClassInfoEx",  
    "GdiCreateClieObj",  
    "UserUnregisterClass",  
    "UserDeleteMenu",  
    "GdiRectInRegion",  
    "UserScrollWindowEx",  
    "GdiGetPixel",  
    "UserSetClassLong",  
    "UserGetMenuBarInfo",  
    "GdiDdCreateSurfaceEx",  
    "GdiDdCreateSurfaceObject",  
    "GdiGetNearestPaletteIndex",  
    "GdiDdLockD3D",  
    "GdiDdUnlockD3D",  
    "GdiGetCharWidthW",  
    "UserInvalidateRgn",  
    "UserGetClipboardOwner",  
    "UserSetWindowRgn",  
    "UserBitBltSysBmp",  
    "GdiGetCharWidthInfo",  
    "UserValidateRect",  
    "UserCloseClipboard",  
    "UserOpenClipboard",  
    "GdiGetStockObject",  
    "UserSetClipboardData",  
    "UserEnableMenuItem",  
    "UserAlterWindowStyle",  
    "GdiFillRgn",  
    "UserGetWindowPlaceme",  
    "GdiModifyWorldTransform",  
    "GdiGetFoData",  
    "UserGetOpenClipboardWindow",  
    "UserSetThreadState",  
    "GdiOpenDCW",  
    "UserTrackMouseEve",  
    "GdiGetTransform",  
    "UserDestroyMenu",  
    "GdiGetBitmapBits",  
    "UserConsoleCorol",  
    "UserSetActiveWindow",  
    "UserSetInformatiohread",  
    "UserSetWindowPlaceme",  
    "UserGetCorolColor",  
    "GdiSetMetaRgn",  
    "GdiSetMiterLimit",  
    "GdiSetVirtualResolution",  
    "GdiGetRasterizerCaps",  
    "UserSetWindowWord",  
    "UserGetClipboardFormatName",  
    "UserRealIernalGetMessage",  
    "UserCreateLocalMemHandle",  
    "UserAttachThreadInput",  
    "GdiCreateHalftonePalette",  
    "UserPaiMenuBar",  
    "UserSetKeyboardState",  
    "GdiCombineTransform",  
    "UserCreateAcceleratorTable",  
    "UserGetCursorFrameInfo",  
    "UserGetAltTabInfo",  
    "UserGetCaretBlinkTime",  
    "GdiQueryFoAssocInfo",  
    "UserProcessConnect",  
    "UserEnumDisplayDevices",  
    "UserEmptyClipboard",  
    "UserGetClipboardData",  
    "UserRemoveMenu",  
    "GdiSetBoundsRect",  
    "GdiGetBitmapDimension",  
    "UserConvertMemHandle",  
    "UserDestroyAcceleratorTable",  
    "UserGetGUIThreadInfo",  
    "GdiCloseFigure",  
    "UserSetWindowsHookAW",  
    "UserSetMenuDefaultItem",  
    "UserCheckMenuItem",  
    "UserSetWinEveHook",  
    "UserUnhookWinEve",  
    "UserLockWindowUpdate",  
    "UserSetSystemMenu",  
    "UserThunkedMenuInfo",  
    "GdiBeginPath",  
    "GdiEndPath",  
    "GdiFillPath",  
    "UserCallHwnd",  
    "UserDdeInitialize",  
    "UserModifyUserStartupInfoFlags",  
    "UserCouClipboardFormats",  
    "GdiAddFoMemResourceEx",  
    "GdiEqualRgn",  
    "GdiGetSystemPaletteUse",  
    "GdiRemoveFoMemResourceEx",  
    "UserEnumDisplaySettings",  
    "UserPaiDesktop",  
    "GdiExtEscape",  
    "GdiSetBitmapDimension",  
    "GdiSetFoEnumeration",  
    "UserChangeClipboardChain",  
    "UserSetClipboardViewer",  
    "UserShowWindowAsync",  
    "GdiCreateColorSpace",  
    "GdiDeleteColorSpace",  
    "UserActivateKeyboardLayout",  
    "GdiAbortDoc",  
    "GdiAbortPath",  
    "GdiAddEmbFoToDC",  
    "GdiAddFoResourceW",  
    "GdiAddRemoteFoToDC",  
    "GdiAddRemoteMMInstanceToDC",  
    "GdiAngleArc",  
    "GdiAnyLinkedFos",  
    "GdiArcIernal",  
    "GdiBRUSHOBJ_DeleteRbrush",  
    "GdiBRUSHOBJ_hGetColorTransform",  
    "GdiBRUSHOBJ_pvAllocRbrush",  
    "GdiBRUSHOBJ_pvGetRbrush",  
    "GdiBRUSHOBJ_ulGetBrushColor",  
    "GdiBeginGdiRendering",  
    "GdiCLIPOBJ_bEnum",  
    "GdiCLIPOBJ_cEnumStart",  
    "GdiCLIPOBJ_ppoGetPath",  
    "GdiCancelDC",  
    "GdiChangeGhostFo",  
    "GdiCheckBitmapBits",  
    "GdiClearBitmapAttributes",  
    "GdiClearBrushAttributes",  
    "GdiColorCorrectPalette",  
    "GdiConfigureOPMProtectedOutput",  
    "GdiConvertMetafileRect",  
    "GdiCreateBitmapFromDxSurface",  
    "GdiCreateColorTransform",  
    "GdiCreateEllipticRgn",  
    "GdiCreateHatchBrushIernal",  
    "GdiCreateMetafileDC",  
    "GdiCreateOPMProtectedOutputs",  
    "GdiCreateRoundRectRgn",  
    "GdiCreateServerMetaFile",  
    "GdiD3dCoextCreate",  
    "GdiD3dCoextDestroy",  
    "GdiD3dCoextDestroyAll",  
    "GdiD3dValidateTextureStageState",  
    "GdiDDCCIGetCapabilitiesString",  
    "GdiDDCCIGetCapabilitiesStringLength",  
    "GdiDDCCIGetTimingReport",  
    "GdiDDCCIGetVCPFeature",  
    "GdiDDCCISaveCurreSettings",  
    "GdiDDCCISetVCPFeature",  
    "GdiDdAddAttachedSurface",  
    "GdiDdAlphaBlt",  
    "GdiDdAttachSurface",  
    "GdiDdBeginMoCompFrame",  
    "GdiDdCanCreateD3DBuffer",  
    "GdiDdColorCorol",  
    "GdiDdCreateD3DBuffer",  
    "GdiDdCreateDirectDrawObject",  
    "GdiDdCreateFullscreenSprite",  
    "GdiDdCreateMoComp",  
    "GdiDdDDIAcquireKeyedMutex",  
    "GdiDdDDICheckExclusiveOwnership",  
    "GdiDdDDICheckMonitorPowerState",  
    "GdiDdDDICheckOcclusion",  
    "GdiDdDDICheckSharedResourceAccess",  
    "GdiDdDDICheckVidPnExclusiveOwnership",  
    "GdiDdDDICloseAdapter",  
    "GdiDdDDIConfigureSharedResource",  
    "GdiDdDDICreateAllocation",  
    "GdiDdDDICreateCoext",  
    "GdiDdDDICreateDCFromMemory",  
    "GdiDdDDICreateDevice",  
    "GdiDdDDICreateKeyedMutex",  
    "GdiDdDDICreateOverlay",  
    "GdiDdDDICreateSynchronizationObject",  
    "GdiDdDDIDestroyAllocation",  
    "GdiDdDDIDestroyCoext",  
    "GdiDdDDIDestroyDCFromMemory",  
    "GdiDdDDIDestroyDevice",  
    "GdiDdDDIDestroyKeyedMutex",  
    "GdiDdDDIDestroyOverlay",  
    "GdiDdDDIDestroySynchronizationObject",  
    "GdiDdDDIEscape",  
    "GdiDdDDIFlipOverlay",  
    "GdiDdDDIGetCoextSchedulingPriority",  
    "GdiDdDDIGetDeviceState",  
    "GdiDdDDIGetDisplayModeList",  
    "GdiDdDDIGetMultisampleMethodList",  
    "GdiDdDDIGetOverlayState",  
    "GdiDdDDIGetPreseHistory",  
    "GdiDdDDIGetPreseQueueEve",  
    "GdiDdDDIGetProcessSchedulingPriorityClass",  
    "GdiDdDDIGetRuimeData",  
    "GdiDdDDIGetScanLine",  
    "GdiDdDDIGetSharedPrimaryHandle",  
    "GdiDdDDIInvalidateActiveVidPn",  
    "GdiDdDDILock",  
    "GdiDdDDIOpenAdapterFromDeviceName",  
    "GdiDdDDIOpenAdapterFromHdc",  
    "GdiDdDDIOpenKeyedMutex",  
    "GdiDdDDIOpenResource",  
    "GdiDdDDIOpenSynchronizationObject",  
    "GdiDdDDIPollDisplayChildren",  
    "GdiDdDDIPrese",  
    "GdiDdDDIQueryAdapterInfo",  
    "GdiDdDDIQueryAllocationResidency",  
    "GdiDdDDIQueryResourceInfo",  
    "GdiDdDDIQueryStatistics",  
    "GdiDdDDIReleaseKeyedMutex",  
    "GdiDdDDIReleaseProcessVidPnSourceOwners",  
    "GdiDdDDIRender",  
    "GdiDdDDISetAllocationPriority",  
    "GdiDdDDISetCoextSchedulingPriority",  
    "GdiDdDDISetDisplayMode",  
    "GdiDdDDISetDisplayPrivateDriverFormat",  
    "GdiDdDDISetGammaRamp",  
    "GdiDdDDISetProcessSchedulingPriorityClass",  
    "GdiDdDDISetQueuedLimit",  
    "GdiDdDDISetVidPnSourceOwner",  
    "GdiDdDDISharedPrimaryLockNotification",  
    "GdiDdDDISharedPrimaryUnLockNotification",  
    "GdiDdDDISignalSynchronizationObject",  
    "GdiDdDDIUnlock",  
    "GdiDdDDIUpdateOverlay",  
    "GdiDdDDIWaitForIdle",  
    "GdiDdDDIWaitForSynchronizationObject",  
    "GdiDdDDIWaitForVerticalBlankEve",  
    "GdiDdDeleteDirectDrawObject",  
    "GdiDdDestroyD3DBuffer",  
    "GdiDdDestroyFullscreenSprite",  
    "GdiDdDestroyMoComp",  
    "GdiDdEndMoCompFrame",  
    "GdiDdFlip",  
    "GdiDdFlipToGDISurface",  
    "GdiDdGetAvailDriverMemory",  
    "GdiDdGetBltStatus",  
    "GdiDdGetDC",  
    "GdiDdGetDriverInfo",  
    "GdiDdGetDriverState",  
    "GdiDdGetDxHandle",  
    "GdiDdGetFlipStatus",  
    "GdiDdGetIernalMoCompInfo",  
    "GdiDdGetMoCompBuffInfo",  
    "GdiDdGetMoCompFormats",  
    "GdiDdGetMoCompGuids",  
    "GdiDdGetScanLine",  
    "GdiDdLock",  
    "GdiDdNotifyFullscreenSpriteUpdate",  
    "GdiDdQueryDirectDrawObject",  
    "GdiDdQueryMoCompStatus",  
    "GdiDdQueryVisRgnUniqueness",  
    "GdiDdReenableDirectDrawObject",  
    "GdiDdReleaseDC",  
    "GdiDdRenderMoComp",  
    "GdiDdSetColorKey",  
    "GdiDdSetExclusiveMode",  
    "GdiDdSetGammaRamp",  
    "GdiDdSetOverlayPosition",  
    "GdiDdUnattachSurface",  
    "GdiDdUnlock",  
    "GdiDdUpdateOverlay",  
    "GdiDdWaitForVerticalBlank",  
    "GdiDeleteColorTransform",  
    "GdiDescribePixelFormat",  
    "GdiDestroyOPMProtectedOutput",  
    "GdiDestroyPhysicalMonitor",  
    "GdiDoBanding",  
    "GdiDrawEscape",  
    "GdiDvpAcquireNotification",  
    "GdiDvpCanCreateVideoPort",  
    "GdiDvpColorCorol",  
    "GdiDvpCreateVideoPort",  
    "GdiDvpDestroyVideoPort",  
    "GdiDvpFlipVideoPort",  
    "GdiDvpGetVideoPortBandwidth",  
    "GdiDvpGetVideoPortConnectInfo",  
    "GdiDvpGetVideoPortField",  
    "GdiDvpGetVideoPortFlipStatus",  
    "GdiDvpGetVideoPortInputFormats",  
    "GdiDvpGetVideoPortLine",  
    "GdiDvpGetVideoPortOutputFormats",  
    "GdiDvpGetVideoSignalStatus",  
    "GdiDvpReleaseNotification",  
    "GdiDvpUpdateVideoPort",  
    "GdiDvpWaitForVideoPortSync",  
    "GdiDxgGenericThunk",  
    "GdiEllipse",  
    "GdiEnableEudc",  
    "GdiEndDoc",  
    "GdiEndGdiRendering",  
    "GdiEndPage",  
    "GdiEngAlphaBlend",  
    "GdiEngAssociateSurface",  
    "GdiEngBitBlt",  
    "GdiEngCheckAbort",  
    "GdiEngComputeGlyphSet",  
    "GdiEngCopyBits",  
    "GdiEngCreateBitmap",  
    "GdiEngCreateClip",  
    "GdiEngCreateDeviceBitmap",  
    "GdiEngCreateDeviceSurface",  
    "GdiEngCreatePalette",  
    "GdiEngDeleteClip",  
    "GdiEngDeletePalette",  
    "GdiEngDeletePath",  
    "GdiEngDeleteSurface",  
    "GdiEngEraseSurface",  
    "GdiEngFillPath",  
    "GdiEngGradieFill",  
    "GdiEngLineTo",  
    "GdiEngLockSurface",  
    "GdiEngMarkBandingSurface",  
    "GdiEngPai",  
    "GdiEngPlgBlt",  
    "GdiEngStretchBlt",  
    "GdiEngStretchBltROP",  
    "GdiEngStrokeAndFillPath",  
    "GdiEngStrokePath",  
    "GdiEngTextOut",  
    "GdiEngTranspareBlt",  
    "GdiEngUnlockSurface",  
    "GdiEnumFos",  
    "GdiEnumObjects",  
    "GdiEudcLoadUnloadLink",  
    "GdiExtFloodFill",  
    "GdiFOOBJ_cGetAllGlyphHandles",  
    "GdiFOOBJ_cGetGlyphs",  
    "GdiFOOBJ_pQueryGlyphAttrs",  
    "GdiFOOBJ_pfdg",  
    "GdiFOOBJ_pifi",  
    "GdiFOOBJ_pvTrueTypeFoFile",  
    "GdiFOOBJ_pxoGetXform",  
    "GdiFOOBJ_vGetInfo",  
    "GdiFlattenPath",  
    "GdiFoIsLinked",  
    "GdiForceUFIMapping",  
    "GdiFrameRgn",  
    "GdiFullscreenCorol",  
    "GdiGetBoundsRect",  
    "GdiGetCOPPCompatibleOPMInformation",  
    "GdiGetCertificate",  
    "GdiGetCertificateSize",  
    "GdiGetCharABCWidthsW",  
    "GdiGetCharacterPlacemeW",  
    "GdiGetColorAdjustme",  
    "GdiGetColorSpaceforBitmap",  
    "GdiGetDeviceCaps",  
    "GdiGetDeviceCapsAll",  
    "GdiGetDeviceGammaRamp",  
    "GdiGetDeviceWidth",  
    "GdiGetDhpdev",  
    "GdiGetETM",  
    "GdiGetEmbUFI",  
    "GdiGetEmbedFos",  
    "GdiGetEudcTimeStampEx",  
    "GdiGetFoFileData",  
    "GdiGetFoFileInfo",  
    "GdiGetFoResourceInfoIernalW",  
    "GdiGetFoUnicodeRanges",  
    "GdiGetGlyphIndicesW",  
    "GdiGetGlyphIndicesWIernal",  
    "GdiGetGlyphOutline",  
    "GdiGetKerningPairs",  
    "GdiGetLinkedUFIs",  
    "GdiGetMiterLimit",  
    "GdiGetMonitorID",  
    "GdiGetNumberOfPhysicalMonitors",  
    "GdiGetOPMInformation",  
    "GdiGetOPMRandomNumber",  
    "GdiGetObjectBitmapHandle",  
    "GdiGetPath",  
    "GdiGetPerBandInfo",  
    "GdiGetPhysicalMonitorDescription",  
    "GdiGetPhysicalMonitors",  
    "GdiGetRealizationInfo",  
    "GdiGetServerMetaFileBits",  
    "DxgStubAlphaBlt",  
    "GdiGetStats",  
    "GdiGetStringBitmapW",  
    "GdiGetSuggestedOPMProtectedOutputArraySize",  
    "GdiGetTextExteExW",  
    "GdiGetUFI",  
    "GdiGetUFIPathname",  
    "GdiGradieFill",  
    "GdiHLSurfGetInformation",  
    "GdiHLSurfSetInformation",  
    "GdiHT_Get8BPPFormatPalette",  
    "GdiHT_Get8BPPMaskPalette",  
    "GdiIcmBrushInfo",  
    "EngRestoreFloatingPoiState",  
    "GdiInitSpool",  
    "GdiMakeFoDir",  
    "GdiMakeInfoDC",  
    "GdiMakeObjectUnXferable",  
    "GdiMakeObjectXferable",  
    "GdiMirrorWindowOrg",  
    "GdiMonoBitmap",  
    "GdiMoveTo",  
    "GdiOffsetClipRgn",  
    "GdiPATHOBJ_bEnum",  
    "GdiPATHOBJ_bEnumClipLines",  
    "GdiPATHOBJ_vEnumStart",  
    "GdiPATHOBJ_vEnumStartClipLines",  
    "GdiPATHOBJ_vGetBounds",  
    "GdiPathToRegion",  
    "GdiPlgBlt",  
    "GdiPolyDraw",  
    "GdiPolyTextOutW",  
    "GdiPtInRegion",  
    "GdiPtVisible",  
    "GdiQueryFos",  
    "GdiRemoveFoResourceW",  
    "GdiRemoveMergeFo",  
    "GdiResetDC",  
    "GdiResizePalette",  
    "GdiRoundRect",  
    "GdiSTROBJ_bEnum",  
    "GdiSTROBJ_bEnumPositionsOnly",  
    "GdiSTROBJ_bGetAdvanceWidths",  
    "GdiSTROBJ_dwGetCodePage",  
    "GdiSTROBJ_vEnumStart",  
    "GdiScaleViewportExtEx",  
    "GdiScaleWindowExtEx",  
    "GdiSelectBrush",  
    "GdiSelectClipPath",  
    "GdiSelectPen",  
    "GdiSetBitmapAttributes",  
    "GdiSetBrushAttributes",  
    "GdiSetColorAdjustme",  
    "GdiSetColorSpace",  
    "GdiSetDeviceGammaRamp",  
    "GdiSetFoXform",  
    "GdiSetIcmMode",  
    "GdiSetLinkedUFIs",  
    "GdiSetMagicColors",  
    "GdiSetOPMSigningKeyAndSequenceNumbers",  
    "GdiSetPUMPDOBJ",  
    "GdiSetPixelFormat",  
    "GdiSetRectRgn",  
    "GdiSetSizeDevice",  
    "GdiSetSystemPaletteUse",  
    "GdiSetTextJustification",  
    "GdiSfmGetNotificatiookens",  
    "GdiStartDoc",  
    "GdiStartPage",  
    "GdiStrokeAndFillPath",  
    "GdiStrokePath",  
    "GdiSwapBuffers",  
    "GdiTranspareBlt",  
    "GdiUMPDEngFreeUserMem",  
    "DxgStubAlphaBlt",  
    "EngRestoreFloatingPoiState",  
    "GdiUpdateColors",  
    "GdiUpdateTransform",  
    "GdiWidenPath",  
    "GdiXFORMOBJ_bApplyXform",  
    "GdiXFORMOBJ_iGetXform",  
    "GdiXLATEOBJ_cGetPalette",  
    "GdiXLATEOBJ_hGetColorTransform",  
    "GdiXLATEOBJ_iXlate",  
    "UserAddClipboardFormatListener",  
    "UserAssociateInputCoext",  
    "UserBlockInput",  
    "UserBuildHimcList",  
    "UserBuildPropList",  
    "UserCalculatePopupWindowPosition",  
    "UserCallHwndOpt",  
    "UserChangeDisplaySettings",  
    "UserChangeWindowMessageFilterEx",  
    "UserCheckAccessForIegrityLevel",  
    "UserCheckDesktopByThreadId",  
    "UserCheckWindowThreadDesktop",  
    "UserChildWindowFromPoiEx",  
    "UserClipCursor",  
    "UserCreateDesktopEx",  
    "UserCreateInputCoext",  
    "UserCreateWindowStation",  
    "UserCtxDisplayIOCtl",  
    "UserDestroyInputCoext",  
    "UserDisableThreadIme",  
    "UserDisplayConfigGetDeviceInfo",  
    "UserDisplayConfigSetDeviceInfo",  
    "UserDoSoundConnect",  
    "UserDoSoundDisconnect",  
    "UserDragDetect",  
    "UserDragObject",  
    "UserDrawAnimatedRects",  
    "UserDrawCaption",  
    "UserDrawCaptioemp",  
    "UserDrawMenuBarTemp",  
    "UserDwmStartRedirection",  
    "UserDwmStopRedirection",  
    "UserEndMenu",  
    "UserEndTouchOperation",  
    "UserEve",  
    "UserFlashWindowEx",  
    "UserFrostCrashedWindow",  
    "UserGetAppImeLevel",  
    "UserGetCaretPos",  
    "UserGetClipCursor",  
    "UserGetClipboardViewer",  
    "UserGetComboBoxInfo",  
    "UserGetCursorInfo",  
    "UserGetDisplayConfigBufferSizes",  
    "UserGetGestureConfig",  
    "UserGetGestureExtArgs",  
    "UserGetGestureInfo",  
    "UserGetGuiResources",  
    "UserGetImeHotKey",  
    "UserGetImeInfoEx",  
    "UserGetInputLocaleInfo",  
    "UserGetIernalWindowPos",  
    "UserGetKeyNameText",  
    "UserGetKeyboardLayoutName",  
    "UserGetLayeredWindowAttributes",  
    "UserGetListBoxInfo",  
    "UserGetMenuIndex",  
    "UserGetMenuItemRect",  
    "UserGetMouseMovePoisEx",  
    "UserGetPriorityClipboardFormat",  
    "UserGetRawInputBuffer",  
    "UserGetRawInputData",  
    "UserGetRawInputDeviceInfo",  
    "UserGetRawInputDeviceList",  
    "UserGetRegisteredRawInputDevices",  
    "UserGetTopLevelWindow",  
    "UserGetTouchInputInfo",  
    "UserGetUpdatedClipboardFormats",  
    "UserGetWOWClass",  
    "UserGetWindowCompositionAttribute",  
    "UserGetWindowCompositionInfo",  
    "UserGetWindowDisplayAffinity",  
    "UserGetWindowMinimizeRect",  
    "UserGetWindowRgnEx",  
    "UserGhostWindowFromHungWindow",  
    "UserHardErrorCorol",  
    "UserHiliteMenuItem",  
    "UserHungWindowFromGhostWindow",  
    "UserHwndQueryRedirectionInfo",  
    "UserHwndSetRedirectionInfo",  
    "UserImpersonateDdeClieWindow",  
    "UserInitTask",  
    "UserInitialize",  
    "UserInitializeCliePfnArrays",  
    "UserInjectGesture",  
    "UserIernalGetWindowIcon",  
    "UserIsTopLevelWindow",  
    "UserIsTouchWindow",  
    "UserLoadKeyboardLayoutEx",  
    "UserLockWindowStation",  
    "UserLockWorkStation",  
    "UserLogicalToPhysicalPoi",  
    "UserMNDragLeave",  
    "UserMNDragOver",  
    "UserMagCorol",  
    "UserMagGetCoextInformation",  
    "UserMagSetCoextInformation",  
    "UserManageGestureHandlerWindow",  
    "UserMenuItemFromPoi",  
    "UserMinMaximize",  
    "UserModifyWindowTouchCapability",  
    "UserNotifyIMEStatus",  
    "UserOpenInputDesktop",  
    "UserOpehreadDesktop",  
    "UserPaiMonitor",  
    "UserPhysicalToLogicalPoi",  
    "UserPriWindow",  
    "UserQueryDisplayConfig",  
    "UserQueryInformatiohread",  
    "UserQueryInputCoext",  
    "UserQuerySendMessage",  
    "UserRealChildWindowFromPoi",  
    "UserRealWaitMessageEx",  
    "UserRegisterErrorReportingDialog",  
    "UserRegisterHotKey",  
    "UserRegisterRawInputDevices",  
    "UserRegisterServicesProcess",  
    "UserRegisterSessionPort",  
    "UserRegisterTasklist",  
    "UserRegisterUserApiHook",  
    "UserRemoteConnect",  
    "UserRemoteRedrawRectangle",  
    "UserRemoteRedrawScreen",  
    "UserRemoteStopScreenUpdates",  
    "UserRemoveClipboardFormatListener",  
    "UserResolveDesktopForWOW",  
    "UserSendTouchInput",  
    "UserSetAppImeLevel",  
    "UserSetChildWindowNoActivate",  
    "UserSetClassWord",  
    "UserSetCursorCoes",  
    "UserSetDisplayConfig",  
    "UserSetGestureConfig",  
    "UserSetImeHotKey",  
    "UserSetImeInfoEx",  
    "UserSetImeOwnerWindow",  
    "UserSetIernalWindowPos",  
    "UserSetLayeredWindowAttributes",  
    "UserSetMenu",  
    "UserSetMenuCoextHelpId",  
    "UserSetMenuFlagRtoL",  
    "UserSetMirrorRendering",  
    "UserSetObjectInformation",  
    "UserSetProcessDPIAware",  
    "UserSetShellWindowEx",  
    "UserSetSysColors",  
    "UserSetSystemCursor",  
    "UserSetSystemTimer",  
    "UserSetThreadLayoutHandles",  
    "UserSetWindowCompositionAttribute",  
    "UserSetWindowDisplayAffinity",  
    "UserSetWindowRgnEx",  
    "UserSetWindowStationUser",  
    "UserSfmDestroyLogicalSurfaceBinding",  
    "UserSfmDxBindSwapChain",  
    "UserSfmDxGetSwapChainStats",  
    "UserSfmDxOpenSwapChain",  
    "UserSfmDxQuerySwapChainBindingStatus",  
    "UserSfmDxReleaseSwapChain",  
    "UserSfmDxReportPendingBindingsToDwm",  
    "UserSfmDxSetSwapChainBindingStatus",  
    "UserSfmDxSetSwapChainStats",  
    "UserSfmGetLogicalSurfaceBinding",  
    "UserShowSystemCursor",  
    "UserSoundSery",  
    "UserSwitchDesktop",  
    "UserTestForIeractiveUser",  
    "UserTrackPopupMenuEx",  
    "UserUnloadKeyboardLayout",  
    "UserUnlockWindowStation",  
    "UserUnregisterHotKey",  
    "UserUnregisterSessionPort",  
    "UserUnregisterUserApiHook",  
    "UserUpdateInputCoext",  
    "UserUpdateInstance",  
    "UserUpdateLayeredWindow",  
    "UserUpdatePerUserSystemParameters",  
    "UserUpdateWindowTransform",  
    "UserUserHandleGraAccess",  
    "UserValidateHandleSecure",  
    "UserWaitForInputIdle",  
    "UserWaitForMsgAndEve",  
    "UserWindowFromPhysicalPoi",  
    "UserYieldTask",  
    "UserSetClassLongPtr",  
    "UserSetWindowLongPtr"
};

const struct _PROCEDURE_NAMES procedureNames[46] = {
    {"CmpKeyObjectType", NULL, NULL, "CmpCloseKeyObject", "CmpDeleteKeyObject", "CmpParseKey", "CmpSecurityMethod", "CmpQueryKeyName", NULL},
    {"IoFileObjectType", NULL, NULL, "IopCloseFile", "IopDeleteFile", "IopParseFile", "IopGetSetSecurityObject", "IopQueryName", NULL},
    {"IoDriverObjectType", NULL, NULL, NULL, "IopDeleteDriver", NULL, NULL, NULL, NULL},
    {"IoDeviceObjectType", NULL, NULL, NULL, "IopDeleteDevice", "IopParseDevice", "IopGetSetSecurityObject", NULL, NULL},
    {"IoCompletionObjectType", NULL, NULL, "IopCloseIoCompletion", "IopDeleteIoCompletion", NULL, NULL, NULL, NULL},
    {"PsJobType", NULL, NULL, "PspJobClose", "PspJobDelete", NULL, NULL, NULL, NULL},
    {"PsThreadType", NULL, "PspThreadOpen", NULL, "PspThreadDelete", NULL, NULL, NULL, NULL},
    {"PsProcessType", NULL, "PspProcessOpen", "PspProcessClose", "PspProcessDelete", NULL, NULL, NULL, NULL},
    {"ObpTypeObjectType", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"ObpDirectoryObjectType", NULL, NULL, "ObpCloseDirectoryObject", NULL, NULL, NULL, NULL, NULL},
    {"ObpSymbolicLinkObjectType", NULL, NULL, NULL, "ObpDeleteSymbolicLink", "ObpParseSymbolicLink", NULL, NULL, NULL},
    {"MmSectionObjectType", NULL, NULL, NULL, "MiSectionDelete", NULL, NULL, NULL, NULL},
    {"ExEventObjectType", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"ExMutantObjectType", NULL, NULL, NULL, "ExpDeleteMutant", NULL, NULL, NULL, NULL},
    {"ExSemaphoreObjectType", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"SeTokenObjectType", NULL, NULL, NULL, "SepTokenDeleteMethod", NULL, NULL, NULL, NULL},
    {"AlpcPortObjectType", NULL, "AlpcpOpenPort", "AlpcpClosePort", "AlpcpDeletePort", NULL, NULL, NULL, NULL},
    {"TmTm", NULL, "FsRtlSyncVolumes", "TmpCloseTransactionManager", "TmpDeleteTransactionManager", NULL, NULL, NULL, NULL},
    {"Desktop", NULL, "ExpWin32OpenProcedure", "ExpWin32CloseProcedure", "ExpWin32DeleteProcedure", "ExpWin32ParseProcedure", NULL, NULL, "ExpWin32DeleteProcedure"},
    {"DebugObject", NULL, NULL, "DbgkpCloseObject", "xHalEndOfBoot", NULL, NULL, NULL, NULL},
    {"TpWorkerFactory", NULL, NULL, "ExpCloseWorkerFactory", "ExpDeleteWorkerFactory", NULL, NULL, NULL, NULL},
    {"Adapter", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"EventPair", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"PcwObject", NULL, "OpenProcedure", "CloseProcedure", "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"WmiGuid", NULL, NULL, NULL, "WmipDeleteMethod", NULL, "WmipDeleteMethod", NULL, NULL},
    {"EtwRegistration", NULL, "OpenProcedure", "CloseProcedure", "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"Session", NULL, NULL, NULL, "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"Timer", NULL, NULL, NULL, "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"WindowStation", NULL, "OpenProcedure", "CloseProcedure", "DeleteProcedure", "ParseProcedure", NULL, NULL, "OkayToCloseProcedure"},
    {"Profile", NULL, NULL, NULL, "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"EtwConsumer", NULL, "EtwpOpenRegistrationObject", "EtwpCloseRegistrationObject", "EtwpDeleteRegistrationObject", NULL, NULL, NULL, NULL},
    {"TmTx", NULL, NULL, "TmpCloseTransaction", "TmpDeleteTransaction", NULL, NULL, NULL, NULL},
    {"KeyedEvent", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"Callback", NULL, NULL, NULL, "FsRtlInitializeOplock", NULL, NULL, NULL, NULL},
    {"FilterConnectionPort", NULL, NULL, "CloseProcedure", "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"UserApcReserve", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"Controller", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"IoCompletionReserve", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"TmEn", NULL, NULL, "TmpCloseEnlistment", "TmpDeleteEnlistment", NULL, NULL, NULL, NULL},
    {"FilterCommunicationPort", NULL, NULL, "CloseProcedure", "DeleteProcedure", NULL, NULL, NULL, NULL},
    {"PowerRequest", NULL, NULL, "PopClosePowerRequestObject", NULL, NULL, NULL, NULL, NULL},
    {"TmRm", NULL, "TmpOpenResourceManager", "TmpCloseResourceManager", "TmpDeleteResourceManager", NULL, NULL, NULL, NULL},
};

ULONG procedureRVAs[46][8];

DWORD RvaToFoa(PIMAGE_NT_HEADERS64 ntHeader, PIMAGE_SECTION_HEADER pSection, DWORD RVA) {
	for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSection++) {
		if (RVA >= pSection->VirtualAddress && RVA < pSection->VirtualAddress + pSection->Misc.VirtualSize) {
			return (RVA - pSection->VirtualAddress) + pSection->PointerToRawData;
		}
	}
	return 0;
}

DWORD FoaToRva(PIMAGE_NT_HEADERS64 ntHeaders, PIMAGE_SECTION_HEADER pSection, DWORD FOA) {
	for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, pSection++) {
		if (FOA >= pSection->PointerToRawData && 
			FOA < pSection->PointerToRawData + pSection->SizeOfRawData) {
			return (FOA - pSection->PointerToRawData) + pSection->VirtualAddress;
		}
	}
	return 0;
}

int QueryProcedures(ULONG NameRVA, PBYTE Buffer, PIMAGE_SECTION_HEADER pTargetSection, PIMAGE_SECTION_HEADER pSection, \
					PIMAGE_NT_HEADERS64 ntHeaders, ULONG ObCreateObjectTypeExRVA, PBYTE pString){
	ULONG pTextStartFOA = pTargetSection->PointerToRawData;
	ULONG pTextStartRVA = pTargetSection->VirtualAddress;
	LONG pTextLoadOffset = pTextStartRVA - pTextStartFOA;
	
	PBYTE pStart = Buffer + pTargetSection->PointerToRawData;
	PBYTE pEnd = pStart + pTargetSection->SizeOfRawData;
	
	PVOID pTarget = 0; // FVA
	for(PBYTE i=pStart;i<pEnd-3;i++){
		if (*(PLONG)(i+3) + (ULONG)(i - Buffer) + pTextLoadOffset + 7 == NameRVA){
			if (*(PUSHORT)i == 0x8d48){
				if (*(i+2)==0x15 || *(i+2)==0x05){
					pTarget = i;
					break;
				}
			}
		}		
	}
	if (!pTarget){
		// Global-Unicode-Packed Condition
		ULONG64 StringSize = 0, StringMaxSize = 0;
		/*for(int i=-1;i<32767;i++){
			if(*(PWCHAR)(pString + i) == 0x0000){
				StringSize = i * sizeof(WCHAR);
				break;
			}
		}*/
		StringSize = wcslen((LPCWSTR)pString) * sizeof(WCHAR);
		if(!StringSize){
			return 1;
		}
		StringMaxSize = (StringSize & 0xFFF0) + 0x10;
		ULONG64 StringVA = ntHeaders->OptionalHeader.ImageBase + NameRVA;
		//ULONG64 Para1to2 = (StringSize << 56) | (StringMaxSize << 40);
		ULONG64 Para1to2 = (StringMaxSize << 16) | StringSize;
		
		//pStart = Buffer + pTargetSection->PointerToRawData;
		//pEnd = pStart + pTargetSection->SizeOfRawData;
		ULONG UnicodeStringRVA = 0;
		for(PBYTE i=pStart;i<pEnd-8;i+=0x16){
			if(*(PULONG64)(i+8) == StringVA){
				if(*(PULONG64)(i) == Para1to2){
					UnicodeStringRVA = FoaToRva(ntHeaders, pSection, i-Buffer);
					break;
				}
			}
		}
		if(!UnicodeStringRVA){
			return 1;
		}
		
		for(PBYTE i=pStart;i<pEnd-3;i++){
			if (*(PLONG)(i+3) + (ULONG)(i - Buffer) + pTextLoadOffset + 7 == UnicodeStringRVA){
				if (*(PUSHORT)i == 0x8d48){
					if (*(i+2)==0x0d){
						pTarget = i;
						break;
					}
				}
			}		
		}
		if(!pTarget){
			return 1;
		}
	}
	
	PBYTE pCall = 0, pFuncStart = 0;
	for(PBYTE i=(PBYTE)pTarget;i<pTarget+0x100 && i<pEnd-1;i++){
		if(*i == 0xe8){
			if (*(PLONG)(i+1) + FoaToRva(ntHeaders, pSection, (ULONG)(i - Buffer)) + 5 == ObCreateObjectTypeExRVA){
				pCall = i;
				//pCallLast = pCall - 0x120;
				pFuncStart = pCall - 0x400;
				break;
			}
		}
	}
	
	for(PBYTE i=(PBYTE)pTarget;i>pTarget-0x400 && i>Buffer-4;i--){
		if(*(PULONG)i == 0x8148f3ff && *(PUCHAR)(i+4) == 0xec){
			pFuncStart = i;
			break;
		}
	}
	
	/*for(PBYTE i=(PBYTE)pTarget;i>pTarget-0x100;i--){
		if(*i == 0xe8){
			if (*(PLONG)(i+1) + FoaToRva(ntHeaders, pSection, (ULONG)(i - Buffer)) + 5 == ObCreateObjectTypeExRVA){
				pCallLast = i;
				break;
			}
		}
	}*/
	if(!pCall){
		return 1;
	}
	
	ULONG StackStartOffset = 0;
	for(PBYTE i=pCall;i>=(PBYTE)pTarget-0x20 && i>Buffer-4;i--){
		if(*(PULONG)i == 0x24548d48){
			StackStartOffset = (ULONG)*(PUCHAR)(i+4);
		}
	}
	
	if(!StackStartOffset){
		for(PBYTE i=pCall;i>=(PBYTE)pTarget-0x20 && i>Buffer;i--){
			if(*(PULONG)i == 0x24948d48){
				StackStartOffset = *(PULONG)(i+4);
			}
		}
		
		if(!StackStartOffset){
			
		}
	}
	
	ULONG64 rax = 0, rcx = 0, rbp = 0;
	//ULONG64 DumpProcedure = 0, OpenProcedure = 0, CloseProcedure = 0, DeleteProcedure = 0;
	//ULONG64 ParseProcedure = 0, SecurityProcedure = 0, QueryNameProcedure = 0, OkayToCloseProcedure = 0;
	ULONG64 Procedures[8];
	memset(Procedures, 0, sizeof(Procedures));
	for(PBYTE i=pFuncStart;i<pCall-4;i++){
		if(*(PUSHORT)i == 0x8d48){
			LONG OffsetProcedure = *(PLONG)(i+3);
			ULONG64 Procedure = OffsetProcedure+i-Buffer+pTextLoadOffset+7;
			switch (*(PUCHAR)(i+2)) {
				case 0x05: {
					rax = Procedure;
					break;
				}
				case 0x0d: {
					rcx = Procedure;
					break;
				}
				case 0x2d: {
					rbp = Procedure;
					break;
				}
			}
		}
		/*if(i<pCallLast){
			continue;
		}*/
		if(*(PUSHORT)i == 0x8948 && *(PUCHAR)(i+3) == 0x24){
			ULONG StackOffset = 0;
			ULONG StructOffset = 0;
			switch (*(PUCHAR)(i+2) & 0xC0) { // 1100 0000
				case 0x40:  // 0100 0000  - 8bit offset
					StackOffset = (ULONG)*(PUCHAR)(i+4);
					break;
				case 0x80:  // 1000 0000  - 32bit offset
					StackOffset = *(PULONG)(i+4);
					break;
			}
			if (StackOffset == 0 || StackOffset < StackStartOffset){
				continue;
			}
			StructOffset = StackOffset - (ULONG)StackStartOffset;
			ULONG64 Procedure = 0;
			if(!(StructOffset & 0x7) && StructOffset >= 0x30 && StructOffset < 0x30 + sizeof(Procedures)){
				switch (*(PUCHAR)(i+2) & 0x3F) {
					case 0x04: {
						Procedure = rax;
						break;
					}
					case 0x0c: {
						Procedure = rcx;
						break;
					}
					case 0x2c: {
						Procedure = rbp;
						break;
					}
					default: continue;
				}		
				Procedures[(StructOffset-0x30) >> 3] = Procedure;	
			}
		}
		
		if (*(PUSHORT)i == 0x8348){
			ULONG StackOffset = 0;
			ULONG StructOffset = 0;
			
			BOOL bZeroOperation = FALSE;
			switch (*(PUCHAR)(i+2)) {
				case 0x64:
					StackOffset = (ULONG)*(PUCHAR)(i+4);
					bZeroOperation = *(PUCHAR)(i+5) == 0;
					break;
				case 0xa4:
					StackOffset = *(PULONG)(i+4);
					bZeroOperation = *(PUCHAR)(i+8) == 0;
					break;
			}
			if(!bZeroOperation){
				continue;
			}
			if (StackOffset == 0 || StackOffset < StackStartOffset){
				continue;
			}
			StructOffset = StackOffset - (ULONG)StackStartOffset;
            if (StructOffset <= 0x30 && StructOffset > 0x30 + sizeof(Procedures)){
                continue;
            }
			Procedures[(StructOffset-0x30) >> 3] = 0;
		}
	}
	
	//printf("0x%llX 0x%llX 0x%llX 0x%llX 0x%llX 0x%llX 0x%llX 0x%llX\n", DumpProcedure, OpenProcedure, CloseProcedure, DeleteProcedure, ParseProcedure, SecurityProcedure, QueryNameProcedure, OkayToCloseProcedure);
	
	for(int i=0;i<8;i++){
		printf("0x%llX ", Procedures[i]);
	}
	printf("\n");
    return 0;
}

int get(){
	CHAR NtOsKrnlPath[MAX_PATH];
	GetSystemDirectoryA(NtOsKrnlPath, MAX_PATH);
	strcat(NtOsKrnlPath, "\\..\\SysNative\\ntoskrnl.exe");

	HANDLE hFile = CreateFileA(NtOsKrnlPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		return FALSE;
	}

	PBYTE Buffer = (PBYTE)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
	if (!Buffer) {
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD bytesRead;
	if (!ReadFile(hFile, Buffer, fileSize, &bytesRead, NULL)) {
		CloseHandle(hFile);
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}

	CloseHandle(hFile);

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)Buffer;
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}

	PIMAGE_NT_HEADERS64 ntHeaders = (PIMAGE_NT_HEADERS64)(Buffer + dosHeader->e_lfanew);
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(ntHeaders);

	
	VirtualFree(Buffer, 0, MEM_RELEASE);
	return 0;
}

BOOL GetOriginDataFromNtOsKrnlImage() {
	QUERY_SSDT_INFORMATION qsinf;
	ULONG64 ssdtNumber = SSDT_NUMBERS;
	DWORD dwReturn;
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_SSDT_DATA, (PVOID)&ssdtNumber, sizeof(ULONG64), (PVOID)&qsinf, sizeof(QUERY_SSDT_INFORMATION), &dwReturn, NULL)) {
		return FALSE;
	}

	CHAR NtOsKrnlPath[MAX_PATH];
	GetSystemDirectoryA(NtOsKrnlPath, MAX_PATH);
	strcat(NtOsKrnlPath, "\\..\\SysNative\\ntoskrnl.exe");

	HANDLE hFile = CreateFileA(NtOsKrnlPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		return FALSE;
	}

	PBYTE Buffer = (PBYTE)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
	if (!Buffer) {
		CloseHandle(hFile);
		return FALSE;
	}

	DWORD bytesRead;
	if (!ReadFile(hFile, Buffer, fileSize, &bytesRead, NULL)) {
		CloseHandle(hFile);
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}

	CloseHandle(hFile);

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)Buffer;
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}

	PIMAGE_NT_HEADERS64 ntHeaders = (PIMAGE_NT_HEADERS64)(Buffer + dosHeader->e_lfanew);
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(ntHeaders);

	ULONG32 KiServiceTableRVA = qsinf.KiServiceTableAddr - qsinf.KernelBase;
	PULONG64 KiServiceTableImage = (PULONG64)(Buffer + RvaToFoa(ntHeaders, pSection, KiServiceTableRVA));
	ULONG64 KernelImageBase = ntHeaders->OptionalHeader.ImageBase;
	//printf("0x%016llX 0x%016llX 0x%08X 0x%08X 0x%08X 0x%016llX\n", qsinf.KiServiceTableAddr, qsinf.KernelBase, KiServiceTableRVA, KiServiceTableFOA, KiServiceTableImage, KernelImageBase);
	for (int i = 0; i < SSDT_NUMBERS; i++) {
		ULONG32 OriginKiServiceTableEntryRVA = KiServiceTableImage[i] - KernelImageBase;
		ULONG64 OriginKiServiceTableEntryVA = OriginKiServiceTableEntryRVA + qsinf.KernelBase;
		//printf("0x%08X 0x%016llX\n", OriginKiServiceTableEntryRVA, OriginKiServiceTableEntryVA);
		OriginSSDTData[i] = OriginKiServiceTableEntryVA;
	}

	PIMAGE_SECTION_HEADER pINIT, pPAGE, pRdata, pText;
	for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, pSection++) {
		if(strcmp((LPCSTR)pSection->Name, "INIT") == 0){
			pINIT = pSection;
			continue;
		}
		if(strcmp((LPCSTR)pSection->Name, "PAGE") == 0){
			pPAGE = pSection;
			continue;
		}
		if(strcmp((LPCSTR)pSection->Name, ".rdata") == 0){
			pRdata = pSection;
			continue;
		}
		if(strcmp((LPCSTR)pSection->Name, ".text") == 0){
			pText = pSection;
			continue;
		}
	}
	pSection = IMAGE_FIRST_SECTION(ntHeaders);
	
	ULONG exportDirFOA = RvaToFoa(ntHeaders, pSection, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if (!exportDirFOA){
		return 1;
	}
	
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(Buffer+exportDirFOA);
	
	DWORD* AddrFunctions = (DWORD*)(Buffer + RvaToFoa(ntHeaders, pSection, ExportDir->AddressOfFunctions));
	DWORD* AddrNames = (DWORD*)(Buffer + RvaToFoa(ntHeaders, pSection, ExportDir->AddressOfNames));
	WORD* AddrOrdinals = (WORD*)(Buffer + RvaToFoa(ntHeaders, pSection, ExportDir->AddressOfNameOrdinals));
	
	ULONG ObCreateObjectTypeFOA = 0;
	for(int i=0;i<ExportDir->NumberOfNames;i++){
		if(strcmp((LPCSTR)(Buffer+RvaToFoa(ntHeaders, pSection, AddrNames[i])), "ObCreateObjectType") == 0){
			ObCreateObjectTypeFOA = RvaToFoa(ntHeaders, pSection, AddrFunctions[AddrOrdinals[i]]);
			break;
			//printf("0x%X\n", AddrFunctions[AddrOrdinals[i]]);
		}
	}
	if(!ObCreateObjectTypeFOA){
		return 1;
	}
	
	ULONG ObCreateObjectTypeExRVA = 0;
	for(PBYTE i=Buffer+ObCreateObjectTypeFOA;i<Buffer+ObCreateObjectTypeFOA+0x20;i++){
		if(*(PULONG)i == 0xe8c93345){
			ObCreateObjectTypeExRVA = (ULONG)(*(PLONG)(i+4)+FoaToRva(ntHeaders, pSection, i-Buffer)+8);
			break;
			//printf("0x%X 0x%X\n", ObCreateObjectTypeExRVA, FoaToRva(ntHeaders, pSection, i-Buffer));
		}
	}
	
	ULONG SeDefaultObjectMethodRVA = 0;
	PBYTE SetDefault = 0;
	PBYTE pObCreateObjectTypeEx = Buffer + RvaToFoa(ntHeaders, pSection, ObCreateObjectTypeExRVA);
	for(PBYTE i=pObCreateObjectTypeEx;i<pObCreateObjectTypeEx+0x1000;i++){
		if(*(PULONG)i == 0x98838948 && *(PUSHORT)(i+4) == 0x0 && *(PUCHAR)(i+6) == 0x0){
			SetDefault = i;
		}
	}
	
	for(PBYTE i=SetDefault;i>(SetDefault-0x40);i--){
		if(*(PUSHORT)i == 0x8d48 && *(PUCHAR)(i+2) == 0x05){
			SeDefaultObjectMethodRVA = (ULONG)(*(PLONG)(i+3)+FoaToRva(ntHeaders, pSection, i-Buffer)+7);
			printf("0x%X\n", SeDefaultObjectMethodRVA);
		}
	}
	
	PBYTE pStart = Buffer + pINIT->PointerToRawData;
	PBYTE pEnd = pStart + pINIT->SizeOfRawData;
	LONG uNameRVA = 0;
	for(PBYTE i=pStart;i<pEnd;i+=0x10){
		PULONG Target = (PULONG)i;
        uNameRVA = i-Buffer-pINIT->PointerToRawData+pINIT->VirtualAddress;
		if(memcmp(Target, L"Process", 16) == 0){
			printf("Process: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Thread\x00\x9090", 16) == 0){
			printf("Thread: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Job\x00\x9090\x9090\x9090\x9090", 16) == 0){
			printf("Job: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Key\x00\x9090\x9090\x9090\x9090", 16) == 0){
			printf("Key: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"File\x00\x9090\x9090\x9090", 16) == 0){
			printf("File: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Device\x00\x9090", 16) == 0){
			printf("Device: ", uNameRVA);
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Driver\x00\x9090", 16) == 0){
			printf("Driver: ", uNameRVA);
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"IoCompletion\x00\x9090\x9090\x9090", 32) == 0){
			printf("IoComplection: ", uNameRVA);
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Directory\x00\x9090\x9090\x9090\x9090\x9090\x9090", 32) == 0){
			printf("Directory: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"SymbolicLink\x00\x9090\x9090", 16) == 0){
			printf("SymbolicName: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Section\x00", 16) == 0){
			printf("Section: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Mutant\x00\x9090", 16) == 0){
			printf("Mutant: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Token\x00\x9090\x9090", 16) == 0){
			printf("Token: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Desktop", 16) == 0){
			printf("Desktop: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"DebugObject\x00\x9090\x9090\x9090\x9090", 32) == 0){
			printf("DebugObject: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"TpWorkerFactory\x0000", 32) == 0){
			printf("TpWorkerFactory: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Session", 16) == 0){
			printf("Session: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Timer\x00\x9090\x9090", 16) == 0){
			printf("Timer: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"WindowStation\x00\x9090\x9090", 32) == 0){
			printf("WindowStation: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"Profile", 16) == 0){
			printf("Profile: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"PowerRequest\x00\x9090\x9090\x9090", 32) == 0){
			printf("PowerRequest: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
	}
	
	for(PBYTE i=pStart;i<pEnd;i++){
		PULONG Target = (PULONG)i;
		if(memcmp(Target, L"WmiGuid", 16) == 0){
			uNameRVA = i-Buffer-pINIT->PointerToRawData+pINIT->VirtualAddress;
			printf("WmiGuid: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			break;
		}
	}
	
	pStart = Buffer + pPAGE->PointerToRawData;
	pEnd = pStart + pPAGE->SizeOfRawData;
	for(PBYTE i=pStart;i<pEnd;i+=16){
		PULONG Target = (PULONG)i;
		if(memcmp(Target, L"ALPC Port\x00\x9090\x9090\x9090\x9090\x9090\x9090\x9090", 32) == 0){
			uNameRVA = i-Buffer-pPAGE->PointerToRawData+pPAGE->VirtualAddress;
			printf("ALPC Port: ");
			QueryProcedures(uNameRVA, Buffer, pPAGE, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"EtwConsumer\x00\x9090\x9090\x9090\x9090", 32) == 0){
			uNameRVA = i-Buffer-pPAGE->PointerToRawData+pPAGE->VirtualAddress;
			printf("EtwConsumer: ");
			QueryProcedures(uNameRVA, Buffer, pPAGE, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
	}
	
	pStart = Buffer + pRdata->PointerToRawData;
	pEnd = pStart + pRdata->SizeOfRawData;
	for(PBYTE i=pStart;i<pEnd;i+=16){
		PULONG Target = (PULONG)i;
		if(memcmp(Target, L"TmTm\x00\x00\x00\x00", 16) == 0){
			uNameRVA = i-Buffer-pRdata->PointerToRawData+pRdata->VirtualAddress;
			printf("TmTm: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"TmTx\x00\x00\x00\x00", 16) == 0){
			uNameRVA = i-Buffer-pRdata->PointerToRawData+pRdata->VirtualAddress;
			printf("TmTx: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"TmEn\x00\x00\x00\x00", 16) == 0){
			uNameRVA = i-Buffer-pRdata->PointerToRawData+pRdata->VirtualAddress;
			printf("TmEn: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
		if(memcmp(Target, L"TmRm\x00\x00\x00\x00", 16) == 0){
			uNameRVA = i-Buffer-pRdata->PointerToRawData+pRdata->VirtualAddress;
			printf("TmRm: ");
			QueryProcedures(uNameRVA, Buffer, pINIT, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
	}
	
	pStart = Buffer + pText->PointerToRawData;
	pEnd = pStart + pText->SizeOfRawData;
	for(PBYTE i=pStart;i<pEnd;i+=16){
		PULONG Target = (PULONG)i;
		if(memcmp(Target, L"EtwRegistration", 32) == 0){
			uNameRVA = i-Buffer-pText->PointerToRawData+pText->VirtualAddress;
			printf("EtwRegistration: ");
			QueryProcedures(uNameRVA, Buffer, pText, pSection, ntHeaders, ObCreateObjectTypeExRVA, i);
			continue;
		}
	}

	VirtualFree(Buffer, 0, MEM_RELEASE);
	return TRUE;
}

BOOL GetOriginSSSDTData(){
	QUERY_SSSDT_INFORMATION qsinf;
	ULONG64 sssdtNumber = SSSDT_NUMBERS;
	DWORD dwReturn;
	if (!DeviceIoControl(hSys, CTL_CODE_QUERY_SSSDT_DATA, (PVOID)&sssdtNumber, sizeof(ULONG64), (PVOID)&qsinf, sizeof(QUERY_SSSDT_INFORMATION), &dwReturn, NULL)) {
		return FALSE;
	}
	
	CHAR Win32kPath[MAX_PATH];
	GetSystemDirectoryA(Win32kPath, MAX_PATH);
	strcat(Win32kPath, "\\..\\SysNative\\win32k.sys");
	
	HANDLE hFile = CreateFileA(Win32kPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	
	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE){
		CloseHandle(hFile);
		return FALSE;
	}
	
	PBYTE Buffer = (PBYTE)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_READWRITE);
	if (!Buffer){
		CloseHandle(hFile);
		return FALSE;
	}
	
	DWORD bytesRead;
	if(!ReadFile(hFile, Buffer, fileSize, &bytesRead, NULL)){
		CloseHandle(hFile);
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	CloseHandle(hFile);
	
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)Buffer;
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE){
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	PIMAGE_NT_HEADERS64 ntHeaders = (PIMAGE_NT_HEADERS64)(Buffer + dosHeader->e_lfanew);
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE){
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(ntHeaders);
	
	/*DWORD importDirRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	if(importDirRVA == 0){
		VirtualFree(Buffer, 0, MEM_RELEASE);
		return FALSE;
	}
	
	PIMAGE_IMPORT_DESCRIPTOR importDir = (PIMAGE_IMPORT_DESCRIPTOR)(Buffer + RvaToFoa(ntHeaders, pSection, importDirRVA));  // 获取 _imp_KeAddSystemServiceTable 的 RVA
	DWORD64 _imp_KeAddSystemServiceTableRVA = 0;
	for(int i=0;importDir[i].Name;i++){
		LPCSTR importDllName = (LPCSTR)(Buffer + RvaToFoa(ntHeaders, pSection, importDir[i].Name));
		if(strcmp(importDllName, "ntoskrnl.exe") == 0){
			PIMAGE_THUNK_DATA64 pIAT = (PIMAGE_THUNK_DATA64)(Buffer + RvaToFoa(ntHeaders, pSection, importDir->FirstThunk));
			for(int j=0;pIAT[j].u1.AddressOfData;j++){
				if ((pIAT[j].u1.AddressOfData && 0x1000000000000000) == 1) continue;
				PIMAGE_IMPORT_BY_NAME pIibn = (PIMAGE_IMPORT_BY_NAME)(Buffer + RvaToFoa(ntHeaders, pSection, pIAT[j].u1.AddressOfData));
				if (strcmp((LPCSTR)&pIibn->Name[0], "KeAddSystemServiceTable") == 0){
					DWORD64 _imp_KeAddSystemServiceTableFOA = (PBYTE)&pIAT[j] - Buffer;
					_imp_KeAddSystemServiceTableRVA = FoaToRva(ntHeaders, pSection, _imp_KeAddSystemServiceTableFOA);
					break;
				}
			}
			break;
		}
	}*/
	
	//ULONG64 _imp_KeAddSystemServiceTableVA = _imp_KeAddSystemServiceTableRVA + qsinf.Win32kBase;
	//DWORD PatternDWORD9 = 0xffffffff && (_imp_KeAddSystemServiceTableRVA + qsinf->Win32kBase);
	
	INT8* pDriverEntry = (INT8*)(Buffer + RvaToFoa(ntHeaders, pSection, ntHeaders->OptionalHeader.AddressOfEntryPoint));
	PULONG64 W32pServiceTable = NULL;
	for(INT8* i=pDriverEntry;i<pDriverEntry + 0x500;i++){		// 从DriverEntry开始搜索w32pServiceTable的引用 0x500字节即可
		UINT8 v0 = *i;
		if (v0 != 0x48) continue;
		
		UINT16 v1, v11;
		UINT32 v7, v13;
		v1 = *(PWORD)(i+1);
		v7 = *(PDWORD)(i+7);
		v11 = *(PWORD)(i+11);
		
		if(v1 == 0x0d8d && v7 == 0x20246c89 && v11 == 0x15ff){
			//INT32 PatternDWORD13 = (ULONG64)i - (ULONG64)Buffer - _imp_KeAddSystemServiceTableRVA;
			
			if(1){
				INT32 v13 = *(PDWORD)(i+13);
				INT32 v3 = *(PDWORD)(i+3);
				DWORD W32pServiceTableRVA = v3 + FoaToRva(ntHeaders, pSection, (DWORD)(i - (DWORD)Buffer)) + 7;
				W32pServiceTable = (PULONG64)(Buffer + RvaToFoa(ntHeaders, pSection, W32pServiceTableRVA));
				//printf("0x%X 0x%X 0x%X\n", v3, W32pServiceTableRVA, W32pServiceTable);
				//CHAR buffer[260];
				//sprintf(buffer, "0x%016llX 0x%016llX 0x%016llX", PatternDWORD13, v13, _imp_KeAddSystemServiceTableRVA);
				//MessageBox(NULL, buffer, "", MB_OK);
				break;
			}
		}
	}
	
	if(!W32pServiceTable){
		return FALSE;
	}
	
	for(int i=0;i<SSSDT_NUMBERS;i++){
		OriginSSSDTData[i] = W32pServiceTable[i] - ntHeaders->OptionalHeader.ImageBase + qsinf.Win32kBase;
	}
	
	VirtualFree(Buffer, 0, MEM_RELEASE);
    return 0;
}