; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!define HOST_AGENT "..\..\..\host_agent"
!define PROXY "..\..\..\proxy"
!define NODE "..\..\..\node"
!define SWORD "..\..\out\${PLATFORM}"

Name "Sword Cloud"
OutFile "${DISTDIR}\sword_cloud_${PLATFORM}_setup.exe"
InstallDir "$PROGRAMFILES\$(^Name)"
InstallDirRegKey HKLM "Software\MASA Group\$(^Name)" "Install_Dir"

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "$(^Name)") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Another installation is in progress"
        Abort
FunctionEnd

;--------------------------------

Section $(^Name)
    SectionIn RO
    SetShellVarContext all

    SetOutPath "$INSTDIR\bin"

    ; cloud applications
    File "${HOST_AGENT}\out\${PLATFORM}\release\samples\sample_agent\sample_agent.exe"
    File "${NODE}\out\jar\node.jar"
    File "${PROXY}\out\jar\log4j.properties"
    File "${PROXY}\out\jar\proxy.jar"

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
    File "${SWORD}\plugins\web_control\web_control_plugin-${PLATFORM}-mt.dll"

    ; modules
    File "${SWORD}\*_module.dll"

    ; website
    SetOutPath "$INSTDIR"
    File /r /x ".svn" "${NODE}\www"

    ; shortcuts
    CreateDirectory "$SMPROGRAMS\$(^Name)"
    CreateShortcut "$SMPROGRAMS\$(^Name)\Cloud Server.lnk" "$INSTDIR\bin\sample_agent.exe" ""

    ; registry
    WriteRegStr HKLM "Software\MASA Group\$(^Name)" "Install_Dir" "$INSTDIR"

    ; VCRedist
    SetOutPath $TEMP
    File "${SWORD}\vcredist_${PLATFORM}.exe"
    ExecWait '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"
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
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    DeleteRegKey HKLM "Software\MASA Group\$(^Name)"
    RmDir /r "$INSTDIR"
    RmDir /r "$SMPROGRAMS\$(^Name)"
SectionEnd
