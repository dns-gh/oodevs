; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

;..................................................................................................
!define APP_NAME "SWORD Urban Emergency - ESRI Arcmap Extension"

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!include AdvUninstLog.nsh
;..................................................................................................

!ifndef OUT_DIR
    !define OUT_DIR "..\..\out"
!endif

!ifndef DISTDIR
    !define DISTDIR "."
!endif

!ifndef VERSION
    !define VERSION "1.0.3135.21232"
!endif

!ifndef PLUGIN_GUID
    !define PLUGIN_GUID "{62C8FE65-4EBB-45E7-B440-6E39B2CDBF29}"
!endif

!ifndef DLL_NAME
    !define DLL_NAME "Sword.Crossbow.dll"
!endif

!ifndef COMPANY
    !define COMPANY "MASA"
!endif

Name "${APP_NAME}"
OutFile "${DISTDIR}\${APP_NAME}.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

LicenseLangString LICENSE ${LANG_ENGLISH} "license-english.txt"
LicenseLangString LICENSE ${LANG_FRENCH} "license-english.txt"


;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "crossbow") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Installer already running"
        Abort
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Function RegisterComponent 
    ;GetDLLVersionLocal "$INSTDIR\Sword.Crossbow.dll"
    
    Pop $R0  ;name of the .NET component, ex: Sword.Crossbow.CreateWorkspace
    Pop $R1  ;GUID
    
    WriteRegStr HKEY_CLASSES_ROOT "$R0" "" "$R0"
    WriteRegStr HKEY_CLASSES_ROOT "$R0\CLSID" "" "$R1"
    
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1" "" "$R0"    
    
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "" "mscoree.dll"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "ThreadingModel" "Both"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "Class" "$R0"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "Assembly" "Sword.Crossbow, Version=${VERSION}, Culture=neutral, PublicKeyToken=null"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "RuntimeVersion" "v2.0.50727"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32" "CodeBase" "$INSTDIR\Sword.Crossbow.dll"
    
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32\${VERSION}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32\${VERSION}" "Class" "$R0"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32\${VERSION}" "Assembly" "Sword.Crossbow, Version=${VERSION}, Culture=neutral, PublicKeyToken=null"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32\${VERSION}" "RuntimeVersion" "v2.0.50727"
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\InprocServer32\${VERSION}" "CodeBase" "$INSTDIR\Sword.Crossbow.dll"
    
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\ProgId" "" "$R0"
    
    ; Application GUID
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\$R1\Implemented Categories\${PLUGIN_GUID}" "" ""    
FunctionEnd

;--------------------------------
Section "!Basic"
    SectionIn RO
    
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    WriteRegStr "${INSTDIR_REG_ROOT}" "Software\${APP_NAME}" "Install_Dir" "$INSTDIR"
    CreateDirectory "$INSTDIR"    
    
    SetOutPath "$INSTDIR"
    ; ${OUT_DIR}
    File "${OUT_DIR}\applications\crossbow_app\vc80\Release\*.dll"    
    File "${OUT_DIR}\applications\crossbow_app\vc80\Release\*.tlb"
        
    ;Sword.Crossbow.CreateWorkspace
    Push "{987E32AA-8556-4E83-AA40-02C799EE734B}"
    Push "Sword.Crossbow.CreateWorkspace"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{987E32AA-8556-4E83-AA40-02C799EE734B}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{987E32AA-8556-4E83-AA40-02C799EE734B}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.AddDynamicMoleLayer
    Push "{603D8BFC-14B0-4A37-A42A-DE1F5C7B4156}"
    Push "Sword.Crossbow.AddDynamicMoleLayer"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{603D8BFC-14B0-4A37-A42A-DE1F5C7B4156}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{603D8BFC-14B0-4A37-A42A-DE1F5C7B4156}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.OrderSelectionMenu
    Push "{1909EFBF-A5DF-470E-A60C-4450FEEAB33D}"
    Push "Sword.Crossbow.OrderSelectionMenu"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{1909EFBF-A5DF-470E-A60C-4450FEEAB33D}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{1909EFBF-A5DF-470E-A60C-4450FEEAB33D}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.EmergencyCreationMenu
    Push "{968BD0CA-4BA6-4AB0-8CAE-EB624B6FF5ED}"
    Push "Sword.Crossbow.EmergencyCreationMenu"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{968BD0CA-4BA6-4AB0-8CAE-EB624B6FF5ED}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{968BD0CA-4BA6-4AB0-8CAE-EB624B6FF5ED}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.SwordExtension
    Push "{1810C551-42E8-4530-BFE0-0FB91D0F634F}"
    Push "Sword.Crossbow.SwordExtension"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{1810C551-42E8-4530-BFE0-0FB91D0F634F}\Implemented Categories\{720E21D6-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{1810C551-42E8-4530-BFE0-0FB91D0F634F}\Implemented Categories\{B56A7C45-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.SwordBar
    Push "{666B9717-13E0-47D0-A388-7B3B7B581342}"
    Push "Sword.Crossbow.SwordBar"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{666B9717-13E0-47D0-A388-7B3B7B581342}\Implemented Categories\{720E21D3-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{666B9717-13E0-47D0-A388-7B3B7B581342}\Implemented Categories\{B56A7C4A-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.CreateEventTool
    Push "{DC81821B-1304-4F9B-B7EC-6846A7A4907C}"
    Push "Sword.Crossbow.CreateEventTool"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{DC81821B-1304-4F9B-B7EC-6846A7A4907C}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{DC81821B-1304-4F9B-B7EC-6846A7A4907C}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.InfoPanelCommand
    Push "{C1A51B6E-BB30-43D9-84E4-989355FED456}"
    Push "Sword.Crossbow.InfoPanelCommand"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{C1A51B6E-BB30-43D9-84E4-989355FED456}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{C1A51B6E-BB30-43D9-84E4-989355FED456}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.InfoPanel
    Push "{E8FC9BE2-8B39-4250-8522-6D061CFC5769}"
    Push "Sword.Crossbow.InfoPanel"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{E8FC9BE2-8B39-4250-8522-6D061CFC5769}\Implemented Categories\{720E21D8-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{E8FC9BE2-8B39-4250-8522-6D061CFC5769}\Implemented Categories\{117623B5-F9D1-11D3-A67F-0008C7DF97B9}" "" ""
    
    ;Sword.Crossbow.Orbat
    Push "{02A14A61-DD9F-493E-80BE-7927AE8A5BAC}"
    Push "Sword.Crossbow.Orbat"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{02A14A61-DD9F-493E-80BE-7927AE8A5BAC}\Implemented Categories\{720E21D8-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{02A14A61-DD9F-493E-80BE-7927AE8A5BAC}\Implemented Categories\{117623B5-F9D1-11D3-A67F-0008C7DF97B9}" "" ""
    
    ;Sword.Crossbow.OrderTool
    Push "{4E5DD04C-E711-467C-A21C-E7F250F0991F}"
    Push "Sword.Crossbow.OrderTool"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{4E5DD04C-E711-467C-A21C-E7F250F0991F}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{4E5DD04C-E711-467C-A21C-E7F250F0991F}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.SwordMenu
    Push "{2EB6AE6D-2B52-40E3-9177-0B2D43A793FE}"
    Push "Sword.Crossbow.SwordMenu"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{2EB6AE6D-2B52-40E3-9177-0B2D43A793FE}\Implemented Categories\{720E21D3-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{2EB6AE6D-2B52-40E3-9177-0B2D43A793FE}\Implemented Categories\{B56A7C4A-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.OrbatCommand
    Push "{3EA5717A-2554-4839-9DAF-667305A11285}"
    Push "Sword.Crossbow.OrbatCommand"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{3EA5717A-2554-4839-9DAF-667305A11285}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{3EA5717A-2554-4839-9DAF-667305A11285}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    ;Sword.Crossbow.FilterViewTool
    Push "{F44A3523-8FAE-4FD8-9895-FA0BDE4EFFF5}"
    Push "Sword.Crossbow.FilterViewTool"
    Call RegisterComponent
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{F44A3523-8FAE-4FD8-9895-FA0BDE4EFFF5}\Implemented Categories\{720E21D4-2199-11D6-B2B3-00508BCDDE28}" "" ""
    WriteRegStr HKEY_CLASSES_ROOT "CLSID\{F44A3523-8FAE-4FD8-9895-FA0BDE4EFFF5}\Implemented Categories\{B56A7C42-83D4-11D2-A2E9-080009B6F22B}" "" ""
    
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

Function UnregisterComponent 
    ;GetDLLVersionLocal "$INSTDIR\Sword.Crossbow.dll"
    
    Pop $R0  ;name of the .NET component, ex: Sword.Crossbow.CreateWorkspace
    Pop $R1  ;GUID    
    DeleteRegKey HKEY_CLASSES_ROOT "$R0"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\$R1"    
FunctionEnd

;--------------------------------
Section UnInstall
    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL
    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"
    RmDir /r "$SMPROGRAMS\${APP_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.CreateWorkspace"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{987E32AA-8556-4E83-AA40-02C799EE734B}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.AddDynamicMoleLayer"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{603D8BFC-14B0-4A37-A42A-DE1F5C7B4156}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.OrderSelectionMenu"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{1909EFBF-A5DF-470E-A60C-4450FEEAB33D}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.EmergencyCreationMenu"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{968BD0CA-4BA6-4AB0-8CAE-EB624B6FF5ED}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.SwordExtension"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{1810C551-42E8-4530-BFE0-0FB91D0F634F}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.SwordBar"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{666B9717-13E0-47D0-A388-7B3B7B581342}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.CreateEventTool"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{DC81821B-1304-4F9B-B7EC-6846A7A4907C}"
        
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.InfoPanelCommand"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{C1A51B6E-BB30-43D9-84E4-989355FED456}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.InfoPanel"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{E8FC9BE2-8B39-4250-8522-6D061CFC5769}"
        
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.Orbat"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{02A14A61-DD9F-493E-80BE-7927AE8A5BAC}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.OrderTool"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{4E5DD04C-E711-467C-A21C-E7F250F0991F}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.SwordMenu"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{2EB6AE6D-2B52-40E3-9177-0B2D43A793FE}"
        
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.OrbatCommand"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{3EA5717A-2554-4839-9DAF-667305A11285}"
    
    DeleteRegKey HKEY_CLASSES_ROOT "Sword.Crossbow.FilterViewTool"
    DeleteRegKey HKEY_CLASSES_ROOT "CLSID\{F44A3523-8FAE-4FD8-9895-FA0BDE4EFFF5}"
    
    ; unregister .otpak extension association
    DeleteRegKey HKCR ".otpak"    
SectionEnd
