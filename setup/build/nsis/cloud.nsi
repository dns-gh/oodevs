; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
;
; ------------------------------------------------------------------------------

!define HOST_AGENT "..\..\..\host_agent"
!define PROXY "..\..\..\proxy"
!define NODE "..\..\..\node"
!define SWORD "${NODE}\out\${PLATFORM}"
!define OUT "..\..\out\${PLATFORM}"
!define EXTERNAL "..\..\external"

!if ${PLATFORM} == "vc100_x64"
!define PRG $PROGRAMFILES64
!else
!define PRG $PROGRAMFILES
!endif

Name "Sword Cloud"
OutFile "${DISTDIR}\sword_cloud_${PLATFORM}_setup.exe"
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

    ; stop current service
    nsExec::Exec 'net stop "Sword Cloud"'

    ; set permissions
    CreateDirectory "$INSTDIR"
    nsExec::Exec 'icacls "$INSTDIR" /t /C /grant Everyone:(OI)(CI)F'

    SetOutPath "$INSTDIR\bin"

    ; cloud applications
    File "${HOST_AGENT}\out\${PLATFORM}\RelWithDebInfo\cloud_server.exe"
    File "${NODE}\out\${PLATFORM}\node.exe"
    File "${PROXY}\out\${PLATFORM}\proxy.exe"

    ; sword applications
    File "${SWORD}\directia-${PLATFORM}-mt-4_6.dll"
    File "${SWORD}\dispatcher-${PLATFORM}-mt.dll"
    File "${SWORD}\functions.xml"
    File "${SWORD}\geos.dll"
    File "${SWORD}\libpq.dll"
    File "${SWORD}\log4cxx.dll"
    File "${SWORD}\lua-${PLATFORM}-mt-5_1_4.dll"
    File "${SWORD}\plugin_communication_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_errorhandler_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\plugin_eventmanager_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\plugin_knowledge_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_masalife_brain_${PLATFORM}-mt.plugin"
    File "${SWORD}\plugin_motivation_${PLATFORM}-mt-4_6.plugin"
    File "${SWORD}\proj.dll"
    File "${SWORD}\simulation_app.exe"
    File "${SWORD}\replayer_app.exe"
    File /x "*_d.dll" "${SWORD}\gdal*.dll"
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

    ; website
    SetOutPath "$INSTDIR\www\js"
    File /r /x ".svn" "${NODE}\www\js\*.js"
    SetOutPath "$INSTDIR\www\css"
    File /r /x ".svn" "${NODE}\www\css\*.css"
    SetOutPath "$INSTDIR\www"
    File /r /x ".svn" "${NODE}\www\*.ttml"
    File /r /x ".svn" "${NODE}\www\img"

    ; shortcuts
    CreateDirectory "$SMPROGRAMS\$(^Name)"
    !insertmacro CreateInternetShortcut "$SMPROGRAMS\$(^Name)\Cluster" "http://localhost:8080/"

    ; registry
    WriteRegStr HKLM "Software\MASA Group\$(^Name)" "Install_Dir" "$INSTDIR"

    ; VCRedist
    SetOutPath $TEMP
    File "${OUT}\vcredist_${PLATFORM}.exe"
    nsExec::Exec '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"

    ; service
    nsExec::Exec '"$INSTDIR\bin\cloud_server.exe" --register'
    nsExec::Exec 'net start "Sword Cloud"'
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
