; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!define ROOT "${OUTDIR}\..\${PLATFORM}"
!define CLOUD "${ROOT}\cloud"
!define SWORD "${ROOT}\sword"

!if ${PLATFORM} == "vc100_x64"
!define PRG $PROGRAMFILES64
!else
!define PRG $PROGRAMFILES
!endif

!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions

Name "Sword Cloud"
OutFile "${DISTDIR}\sword_cloud_setup_${PLATFORM}.exe"
InstallDir "${PRG}\$(^Name)"
InstallDirRegKey HKLM "Software\MASA Group\$(^Name)" "Install_Dir"
RequestExecutionLevel admin

VAR SKIP_SERVICE_CONTROL

;--------------------------------
Function .onInit
    ${GetParameters} $R0
    ClearErrors

    StrCpy $SKIP_SERVICE_CONTROL "true"
    ${GetOptions} $R0 /SKIP_SERVICE_CONTROL $0
    IfErrors 0 +2
    StrCpy $SKIP_SERVICE_CONTROL "false"
    ClearErrors

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

    ${If} $SKIP_SERVICE_CONTROL == "false"
        ; stop current service
        nsExec::Exec 'net stop "Sword Cloud"'
    ${Endif}

    ; set permissions
    CreateDirectory "$INSTDIR"
    nsExec::Exec 'icacls "$INSTDIR" /t /C /grant Everyone:(OI)(CI)F'

    SetOutPath "$INSTDIR\bin"

    ; cloud applications
    File "${CLOUD}\cloud_server.exe"
    File "${CLOUD}\node.exe"
    File "${CLOUD}\proxy.exe"

    ; sword applications
    File "${SWORD}\directia-${PLATFORM}-mt-4_6.dll"
    File "${SWORD}\dispatcher-${PLATFORM}-mt.dll"
    File "${SWORD}\functions.xml"
    File "${SWORD}\libeay32.dll"
    File "${SWORD}\libexpat.dll"
    File "${SWORD}\libiconv.dll"
    File "${SWORD}\libpq.dll"
    File "${SWORD}\log4cxx.dll"
    File "${SWORD}\lua-${PLATFORM}-mt-5_1_4.dll"
    File "${SWORD}\plugin_communication_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_devtools_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\plugin_errorhandler_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\plugin_eventmanager_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\plugin_knowledge_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_masalife_brain_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_motivation_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\proj.dll"
    File "${SWORD}\replayer_app.exe"
    File "${SWORD}\simulation_app.exe"
    File "${SWORD}\spatialite.dll"
    File "${SWORD}\ssleay32.dll"
    File /x "*_d.dll" "${SWORD}\gdal*.dll"
    File /x "*_d.dll" "${SWORD}\geos*.dll"
    File /x "*D.dll" "${SWORD}\Xalan*.dll"
    File /x "*D.dll" "${SWORD}\xerces*.dll"
    File /x "Qt*d.dll" /x "Qt*d4.dll" "${SWORD}\Qt*.dll"
!if ${PLATFORM} == "vc100_x64"
    File "${SWORD}\bugtrap-x64.dll"
!else
    File "${SWORD}\bugtrap.dll"
!endif

    ; resources
    File /r /x ".svn" /x "*.qm" "${SWORD}\resources"

    ; plugins
    File "${SWORD}\plugins\web_control\web_control_plugin.dll"
    File /r /x ".svn" "${SWORD}\plugins"

    ; modules
    File "${SWORD}\*_module.dll"

    ; timeline
    File /r "${RUNDIR}\timeline_server.exe"
    File /r "${RUNDIR}\timeline.pak"

    ; gaming
    SetOutPath "$INSTDIR\client\bin32"
    ; remove metadata.tag with cached crc
    Delete "$INSTDIR\client\bin32\metadata.tag"
    !ifdef SUB_PLATFORM
        File /r "${OUTDIR}\..\${SUB_PLATFORM}\gaming\*"
    !else
        File /r "${OUTDIR}\..\${PLATFORM}\gaming\*"
    !endif

    ; website
    RmDir /r "$INSTDIR\www"
    SetOutPath "$INSTDIR\www\js"
    File /r /x ".svn" "${CLOUD}\www\js\*.js"
    SetOutPath "$INSTDIR\www\css"
    File /r /x ".svn" "${CLOUD}\www\css\*.css"
    SetOutPath "$INSTDIR\www"
    File /r /x ".svn" "${CLOUD}\www\*.ttml"
    File /r /x ".svn" "${CLOUD}\www\img"
    !ifdef SUB_PLATFORM
        File "${OUTDIR}\..\${SUB_PLATFORM}\sword_client_setup_${SUB_PLATFORM}.exe"
    !else
        File "${DISTDIR}\sword_client_setup_${PLATFORM}.exe"
    !endif


    ; shortcuts
    CreateDirectory "$SMPROGRAMS\$(^Name)"
    !insertmacro CreateInternetShortcut "$SMPROGRAMS\$(^Name)\Cluster" "http://localhost:8080/"

    ; registry
    WriteRegStr HKLM "Software\MASA Group\$(^Name)" "Install_Dir" "$INSTDIR"

    ; VCRedist
    SetOutPath $TEMP
    File "${ROOT}\vcredist_${PLATFORM}.exe"
    nsExec::Exec '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"

    ; service
    nsExec::Exec '"$INSTDIR\bin\cloud_server.exe" --register'
    ${If} $SKIP_SERVICE_CONTROL == "false"
        nsExec::Exec 'net start "Sword Cloud"'
    ${Endif}
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
    nsExec::Exec 'net stop "Sword Cloud"'
    nsExec::Exec '"$INSTDIR\bin\cloud_server.exe" --unregister'
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    DeleteRegKey HKLM "Software\MASA Group\$(^Name)"
    RmDir /r "$INSTDIR"
    RmDir /r "$SMPROGRAMS\$(^Name)"
SectionEnd
