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

!include "AdvUninstLog.nsh"
!include "tools.nsh"

!insertmacro OT.Initialize
;..................................................................................................

Name "${APP_NAME}"
OutFile "${DISTDIR}\${APP_NAME} - Demo.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

!include "lang.nsh"
!include "version.nsh"

;--------------------------------
Section "!${APP_NAME}"
    SectionIn RO

    ; readme / changelog files
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\readme.txt" ; $(OT_LANG)
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
    File "${OUTDIR}\release\applications\package_app\*.exe"
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
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Self Training.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\uninstall.lnk" "${UNINST_EXE}"

    !insertmacro OT.AddUninstallEntry    
    !insertmacro OT.AddFileAssoc

SectionEnd

;--------------------------------
SectionGroup "Models" s_mod

    !insertmacro OT.AddDecisionalModels "ada"
    !insertmacro OT.AddPhysicalModels "ada" "worldwide"

SectionGroupEnd

;--------------------------------
SectionGroup "Exercises" s_exo

    !insertmacro OT.AddExercise "Egypt" "Nord egypt" "s_exo1"
    !insertmacro OT.AddExercise "Paris" "Paris_Est" "s_exo2"
    !insertmacro OT.AddExercise "Scripting demo" "test" "s_exo3"

SectionGroupEnd

;--------------------------------
SectionGroup "Terrains" s_ter

    !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
    !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
    !insertmacro OT.AddTerrain "test" "s_ter3"

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

;--------------------------------
Section "Uninstall"

    !insertmacro OT.KillRunning
    !insertmacro OT.Uninstall

SectionEnd

;--------------------------------
Function .onInit

    !insertmacro OT.CheckRunning
    !insertmacro OT.ChooseLanguage

    ; Set section names
    SectionSetText ${s_mod} $(OT_SECTION_MODELS)
    SectionSetText ${s_decmod} $(OT_SECTION_DECISIONAL_MODELS)
    ;SectionSetText ${s_decmodsrc} $(OT_SECTION_DECISIONAL_MODELS_SOURCES)
    SectionSetText ${s_phymod} $(OT_SECTION_PHYSICAL_MODELS)
    SectionSetText ${s_exo} $(OT_SECTION_EXERCISES)
    SectionSetText ${s_ter} $(OT_SECTION_TERRAINS)
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

Function .onSelChange

    !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
    !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo3" "s_ter3"
     
FunctionEnd

