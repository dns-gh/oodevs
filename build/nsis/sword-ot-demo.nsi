; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

;..................................................................................................
!define APP_NAME "SWORD Officer Training - Demo"

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!include AdvUninstLog.nsh
;..................................................................................................

!ifndef PLATFORM
    !define PLATFORM "vc80"
!endif
!ifndef RUNDIR
    !define RUNDIR "..\..\run\${PLATFORM}"
!endif
!ifndef DATADIR
    !define DATADIR "..\..\data"
!endif
!ifndef LIBDIR
    !define LIBDIR "..\..\lib\${PLATFORM}"
!endif
!ifndef OUTDIR
    !define OUTDIR "..\..\out\${PLATFORM}"
!endif
!ifndef DISTDIR
    !define DISTDIR "."
!endif
!ifndef DOCDIR
    !define DOCDIR "..\..\doc"
!endif
!ifndef LIBRARIESDIR
    !define LIBRARIESDIR "..\..\src\libraries"
!endif
!ifndef APPLICATIONSDIR
    !define APPLICATIONSDIR "..\..\src\applications"
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
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "sword-ot-demo") i .r1 ?e'
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

;--------------------------------
Section "!Basic"
    SectionIn RO
    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDIR}\release\applications\adaptation_app\*.exe"
    File "${OUTDIR}\release\applications\gaming_app\*.exe"
    File "${OUTDIR}\release\applications\preparation_app\*.exe"
    File "${OUTDIR}\release\applications\simulation_app\*.exe"
    File "${OUTDIR}\release\applications\replayer_app\*.exe"
    File "${OUTDIR}\release\applications\frontend_app\*.exe"
    File "${OUTDIR}\generation_app\*.exe"
    File "${RUNDIR}\*.qm"
    File "${RUNDIR}\symbols.pak"
    File "${RUNDIR}\symbols.xml"
    File "${RUNDIR}\gradients.xml"
    File "${RUNDIR}\noise.dds"
    File "${RUNDIR}\preferences.xml"
    File "${RUNDIR}\DrawingTemplates.xml"
    File "${RUNDIR}\functions.xml"
    File "${RUNDIR}\*.svg"
    File "${RUNDIR}\TAO.dll"
    File "${RUNDIR}\TAO_CosNaming.dll"
    File "${RUNDIR}\TAO_IORTable.dll"
    File "${RUNDIR}\TAO_PortableServer.dll"
    File "${RUNDIR}\TAO_RTEvent.dll"
    File "${RUNDIR}\TAO_RTSched.dll"
    File "${RUNDIR}\TAO_Strategies.dll"
    File "${RUNDIR}\TAO_Svc_Utils.dll"
    File "${RUNDIR}\ace.dll"
    ;File "${RUNDIR}\dbghelp.dll"
    File "${RUNDIR}\libFedTime.dll"
    File "${RUNDIR}\libRTI-NG.dll"
    File "${RUNDIR}\librtiInternalIntercept.dll"
    File /x "msvc*d.dll" "${RUNDIR}\msvc*.dll"
    File "${RUNDIR}\qt-*.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\population-${PLATFORM}-mt.dll"
    File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\shapelib.dll"
    File /nonfatal "${RUNDIR}\*.manifest"
    File "*.ico"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Adaptation.lnk" "$INSTDIR\applications\adaptation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Frontend.lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\uninstall.lnk" "${UNINST_EXE}"

    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${APP_NAME}"
    ;Same as create shortcut you need to use ${UNINST_EXE} instead of anything else.
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
    
    ; register .otpak extension association
    WriteRegStr HKCR ".otpak" "" "Officer Training Package"
	WriteRegStr HKCR "Officer Training Package\shell" "" "open"
	WriteRegStr HKCR "Officer Training Package\DefaultIcon" "" "$INSTDIR\applications\sword-ot.ico"
	WriteRegStr HKCR "Officer Training Package\shell\open\command" "" '$INSTDIR\applications\frontend_app.exe --install="%1"'
SectionEnd

Section "Terrains"
    SectionIn RO
    SetOutPath "$INSTDIR\data\terrains"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DATADIR}\data\terrains\Nord egypt"
    File /r /x ".svn" "${DATADIR}\data\terrains\Paris_Est"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

Section "Decisional models"
    SectionIn RO
    SetOutPath "$INSTDIR\data\models\ada\decisional"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\*.xml"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

Section "Physical models"
    SectionIn RO
    SetOutPath "$INSTDIR\data\models\ada\physical"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DATADIR}\data\models\ada\physical\worldwide"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

Section "Exercises"
    SectionIn RO
    SetOutPath "$INSTDIR\exercises"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DATADIR}\exercises\Egypt"
    File /r /x ".svn" "${DATADIR}\exercises\Paris"

    CreateDirectory "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme\Readme.lnk" "$INSTDIR\exercises\Egypt\doc\Egypt scenario.txt"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme\Screenshot.lnk" "$INSTDIR\exercises\Egypt\doc\Egypt scenario.png"

    CreateDirectory "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme\Readme.lnk" "$INSTDIR\exercises\Paris\doc\Paris scenario.txt"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme\Screenshot.lnk" "$INSTDIR\exercises\Paris\doc\Paris scenario.png"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

Section "Documentation"
    SectionIn RO
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\*.pdf"
    File /r /x ".svn" "third party"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\User Guide.lnk" "$INSTDIR\doc\User Guide.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

;--------------------------------
Section UnInstall
    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL
    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"
    RmDir /r "$SMPROGRAMS\${APP_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
    
    ; unregister .otpak extension association
    DeleteRegKey HKCR ".otpak"
SectionEnd
