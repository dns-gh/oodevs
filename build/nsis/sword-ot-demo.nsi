; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

;..................................................................................................
!define APP_NAME "SWORD Officer Training"

; defined from ant call
!ifndef APP_VERSION
    !define APP_VERSION "1.0.0.0"
!endif

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

!ifndef INSTDATADIR
    !define INSTDATADIR "$DOCUMENTS\${APP_NAME}"
!endif

Name "${APP_NAME}"
OutFile "${DISTDIR}\${APP_NAME} - Demo.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

!include lang.nsh
!include version.nsh

;--------------------------------
Section "${APP_NAME}"
    SectionIn RO

    ; resources: localization
    SetOutPath "$INSTDIR\applications\resources\locales"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${RUNDIR}\*.qm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    ; resources: documentation
    SetOutPath "$INSTDIR\applications\resources\help"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDIR}\release\applications\adaptation_app\*.exe"
    File "${OUTDIR}\release\applications\gaming_app\*.exe"
    File "${OUTDIR}\release\applications\preparation_app\*.exe"
    File "${OUTDIR}\release\applications\simulation_app\*.exe"
    File "${OUTDIR}\release\applications\replayer_app\*.exe"
    File "${OUTDIR}\release\applications\frontend_app\*.exe"
    File "${OUTDIR}\release\applications\selftraining_app\*.exe"
    File "${OUTDIR}\generation_app\*.exe"
    File "${RUNDIR}\gradients.xml"
    File "${RUNDIR}\preferences.xml"
    File "${RUNDIR}\functions.xml"
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
    File /x "qt-*d.dll" "${RUNDIR}\qt-*.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\population-${PLATFORM}-mt.dll"
    File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\directia-${PLATFORM}-mt-4_5.dll"
    File "${RUNDIR}\shapelib.dll"
    File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
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
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoModify" 1
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoRepair" 1
    
    ; register .otpak extension association
    WriteRegStr HKCR ".otpak" "" "Officer Training Package"
	WriteRegStr HKCR "Officer Training Package\shell" "" "open"
	WriteRegStr HKCR "Officer Training Package\DefaultIcon" "" "$INSTDIR\applications\sword-ot.ico"
	WriteRegStr HKCR "Officer Training Package\shell\open\command" "" '$INSTDIR\applications\frontend_app.exe --install="%1"'
SectionEnd

;--------------------------------
SectionGroup "Models" s_mod

    Section "Decisional" s_decmod
        SectionIn RO
        SetOutPath "${INSTDATADIR}\data\models\ada\decisional"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Binaires"
        File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\*.xml"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd    
    
    ;Section "Sources" s_decmodsrc
    ;    SectionIn RO
    ;    SetOutPath "${INSTDATADIR}\data\models\main\decisional"
    ;    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    ;    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Sources"
    ;    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    ;SectionEnd
    
    Section "Physical" s_phymod
        SectionIn RO
        SetOutPath "${INSTDATADIR}\data\models\ada\physical"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\models\ada\physical\worldwide"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd

SectionGroupEnd

!macro EXERCISES.Install ExerciseName TerrainName
    
    Section "${ExerciseName} - ${TerrainName}"
        SetOutPath "${INSTDATADIR}\exercises\${ExerciseName}"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /x ".svn" "${DATADIR}\exercises\${ExerciseName}\*.xml"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\scripts"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\docs"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\sessions"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
        SetOutPath "${INSTDATADIR}\data\terrains"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\terrains\${TerrainName}"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
    
!macroend

;--------------------------------
SectionGroup "Exercises" s_exo

!insertmacro EXERCISES.Install "Egypt" "Nord egypt"
!insertmacro EXERCISES.Install "Paris" "Paris_Est"

SectionGroupEnd

;--------------------------------
Section "Documentation" s_doc
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\*.pdf"
    File /r /x ".svn" "third party"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\User Guide.lnk" "$INSTDIR\doc\User Guide.pdf"

    ; use online gaming resources
    CreateDirectory "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme\Readme.lnk" "${INSTDATADIR}\exercises\Egypt\doc\Egypt scenario.txt"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Egypt Scenario Readme\Screenshot.lnk" "${INSTDATADIR}\exercises\Egypt\doc\Egypt scenario.png"

    CreateDirectory "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme\Readme.lnk" "${INSTDATADIR}\exercises\Paris\doc\Paris scenario.txt"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Paris Scenario Readme\Screenshot.lnk" "${INSTDATADIR}\exercises\Paris\doc\Paris scenario.png"

    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

SectionGroup "Shortcuts" s_sc

    ;--------------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
    
    ;--------------------------------
    Section "Quick Launch" s_quick
        SetOutPath "$INSTDIR\applications"
        StrCmp $QUICKLAUNCH $TEMP +2
        CreateShortCut "$QUICKLAUNCH\${APP_NAME}.lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd

SectionGroupEnd

Function un.KillRunningApplication

    Processes::FindProcess "$9"
    StrCmp $R0 "0" notrunning
    MessageBox MB_YESNO|MB_ICONQUESTION $(OT_APPLICATION_IS_RUNNING) /SD IDYES IDYES kill
        MessageBox MB_OK $(OT_ABORTING_UNINSTALLATION)
        Abort
    kill:
        Pop $0
        Processes::KillProcess "$9"
    notrunning:
    
FunctionEnd

!macro APPLICATION.KillRunning ApplicationName

    Push $9
    StrCpy $9 "${ApplicationName}"
    Call un.KillRunningApplication
    Pop $9

!macroend

;--------------------------------
Section "Uninstall"
    !insertmacro APPLICATION.KillRunning "adaptation_app.exe"
    !insertmacro APPLICATION.KillRunning "selftraining_app.exe"
	!insertmacro APPLICATION.KillRunning "simulation_app.exe"
	!insertmacro APPLICATION.KillRunning "gaming_app.exe"
	!insertmacro APPLICATION.KillRunning "preparation_app.exe"
	!insertmacro APPLICATION.KillRunning "replayer_app.exe"

    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL
 	
    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"
    
    Delete "$DESKTOP\${APP_NAME}.lnk"
    Delete "$QUICKLAUNCH\${APP_NAME}.lnk"
    RmDir /r "$SMPROGRAMS\${APP_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
    
    ; unregister .otpak extension association
    DeleteRegKey HKCR ".otpak"
SectionEnd

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${APP_NAME}") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_ALREADY_RUNNING)"
        Abort
    Push ""
    Push ${LANG_ENGLISH}
    Push English
    Push ${LANG_FRENCH}
    Push Français
    Push A ; auto count languages
    LangDLL::LangDialog "Installer Language" "Please select the language of the installer"
    Pop $LANGUAGE
    
    ; Set section names
    SectionSetText ${s_mod} $(OT_SECTION_MODELS)
    SectionSetText ${s_decmod} $(OT_SECTION_DECISIONAL_MODELS)
    ;SectionSetText ${s_decmodsrc} $(OT_SECTION_DECISIONAL_MODELS_SOURCES)
    SectionSetText ${s_phymod} $(OT_SECTION_PHYSICAL_MODELS)
    SectionSetText ${s_exo} $(OT_SECTION_EXERCISES)
    SectionSetText ${s_doc} $(OT_SECTION_DOCUMENTATION)
    SectionSetText ${s_sc} $(OT_SECTION_SHORTCUTS)
    SectionSetText ${s_desktop} $(OT_SECTION_DESKTOP_SHORTCUT)
    SectionSetText ${s_quick} $(OT_SECTION_QUICKLAUNCH_SHORTCUT)
    
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Function adobeReader
    Push $0
    ClearErrors
    ReadRegStr $0 HKCR "CLSID\{CA8A9780-280D-11CF-A24D-444553540000}" ""
    IfErrors 0 +3
        MessageBox MB_YESNO|MB_ICONQUESTION "$(OT_INSTALL_ADOBE_READER)" /SD IDNO IDNO +2
            ExecShell "open" "http://www.adobe.com/products/acrobat/readstep2.html"
    Pop $0
FunctionEnd
