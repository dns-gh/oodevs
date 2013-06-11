; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!define ROOT "${OUTDIR}\..\${PLATFORM}"

!if ${PLATFORM} == "vc100_x64"
!define PRG $PROGRAMFILES64
!else
!define PRG $PROGRAMFILES
!endif

Name "Sword Client"
OutFile "${DISTDIR}\sword_client_setup_${PLATFORM}.exe"
InstallDir "${PRG}\$(^Name)"
InstallDirRegKey HKLM "Software\MASA Group\$(^Name)" "Install_Dir"
RequestExecutionLevel admin

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "$(^Name)") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Another installation is in progress"
        Abort
    UserInfo::GetAccountType
    pop $0
    ${If} $0 != "admin"
        MessageBox mb_iconstop "Administrator rights are required to run this installer"
        SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        Quit
    ${EndIf}
FunctionEnd

;--------------------------------

!macro CreateInternetShortcut FILENAME URL
WriteINIStr "${FILENAME}.url" "InternetShortcut" "URL" "${URL}"
!macroend

;--------------------------------

Section $(^Name)
    SectionIn RO
    SetShellVarContext all

    ; set permissions
    CreateDirectory "$INSTDIR"
    nsExec::Exec 'icacls "$INSTDIR" /t /C /grant Everyone:(OI)(CI)F'

    SetOutPath "$INSTDIR\bin"

    ; client application
    File "${ROOT}\cloud\cloud_client.exe"
    File "${ROOT}\sword\QtCore4.dll"
    File "${ROOT}\sword\QtGui4.dll"
    File "${ROOT}\sword\QtNetwork4.dll"

    ; shortcuts
    CreateDirectory "$SMPROGRAMS\$(^Name)"
    CreateShortCut "$SMPROGRAMS\$(^Name)\$(^Name).lnk" "$INSTDIR\bin\cloud_client.exe"

    ; registry
    WriteRegStr HKLM "Software\MASA Group\$(^Name)" "Install_Dir" "$INSTDIR"

    ; VCRedist
    SetOutPath $TEMP
    File "${ROOT}\vcredist_${PLATFORM}.exe"
    nsExec::Exec '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"

    ; service
    nsExec::Exec '"$INSTDIR\bin\cloud_client.exe" --register'
SectionEnd

;--------------------------------
Section
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "DisplayName" "$(^Name)"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" "NoRepair" 1
    WriteUninstaller "Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\$(^Name)\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

;--------------------------------
Section "Uninstall"
    SetShellVarContext all
    nsExec::Exec '"$INSTDIR\bin\cloud_client.exe" --unregister'
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    DeleteRegKey HKLM "Software\MASA Group\$(^Name)"
    RmDir /r "$INSTDIR"
    RmDir /r "$SMPROGRAMS\$(^Name)"
SectionEnd
