; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

;..................................................................................................
!define APP_NAME "SCALPED"

;........................................
; defined from ant call
!ifndef APP_VERSION_MAJOR
    !define APP_VERSION_MAJOR "1.0.0"
!endif

!ifndef APP_VERSION_MINOR
    !define APP_VERSION_MINOR "1.0.0.0"
!endif

; worldwide / france
!ifndef APP_MODEL
    !define APP_MODEL "france"
!endif
;........................................


!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!include "AdvUninstLog.nsh"
!include "lang.nsh"
!include "tools.nsh"

!insertmacro OT.Initialize
;..................................................................................................

Name "${APP_NAME}"
OutFile "${DISTDIR}\${APP_NAME}_${APP_VERSION_MAJOR}.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

!include "version.nsh"

LicenseLangString LICENSE ${LANG_FRENCH} "license-scalped.txt"

;--------------------------------
Section "!${APP_NAME}"
    SectionIn RO

    ; resources: localization
    SetOutPath "$INSTDIR\applications\resources\locales"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /x tools_fr.qm "${RUNDIR}\*.qm"
    Rename "tools_fr_scalped.qm" "tools_fr.qm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    ; resources: documentation
    SetOutPath "$INSTDIR\applications\resources\help\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDOCDIR}\fr\scalped\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    ; readme / changelog files
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\fr\scalped\readme.txt"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDIR}\release\applications\gaming_app\*.exe"
    File "${OUTDIR}\release\applications\preparation_app\*.exe"
    File "${OUTDIR}\release\applications\simulation_app\*.exe"
    File "${OUTDIR}\release\applications\replayer_app\*.exe"
    File "${OUTDIR}\release\applications\frontend_app\*.exe"
    File "${OUTDIR}\release\applications\selftraining_app\*.exe"
    File "${OUTDIR}\release\applications\package_app\*.exe"
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
    File "${RUNDIR}\qt-mt338.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\population-${PLATFORM}-mt.dll"
    File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\directia-${PLATFORM}-mt-4_5.dll"
    File "${RUNDIR}\shapelib.dll"
    File /r /x ".svn" "${RUNDIR}\resources"
    File /nonfatal "${RUNDIR}\*.manifest"
    File "*.ico"
    
    ; scalped licence
    File "scalped.lic"
    
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\$(OT_UNINSTALL).lnk" "${UNINST_EXE}"

    !insertmacro OT.AddUninstallEntry    
    !insertmacro OT.AddFileAssoc
    !insertmacro OT.AddCommonConfig
    
SectionEnd

;--------------------------------
SectionGroup "Models" s_mod

    !insertmacro OT.AddDecisionalModels "ada"
    !insertmacro OT.AddPhysicalModels "ada" "${APP_MODEL}"

SectionGroupEnd

;--------------------------------
SectionGroup "Exercises" s_exo

    !insertmacro OT.AddExercise "esag" "Angers" "s_exo1"
    !insertmacro OT.AddExercise "CENTORSEM" "Paris_Est" "s_exo2"
    !insertmacro OT.AddExercise "puma" "larochelle" "s_exo3"
    !insertmacro OT.AddExercise "Cabourg" "Cabourg" "s_exo17"
    !insertmacro OT.AddExercise "Ares" "Blois" "s_exo18"
    !insertmacro OT.AddExercise "tutorials\01 - Generalites" "Paris_Est" "s_exo4"
    !insertmacro OT.AddExercise "tutorials\02 - Jeu" "Paris_Est" "s_exo5"
    !insertmacro OT.AddExercise "tutorials\03 - Mission" "Paris_Est" "s_exo6"
    !insertmacro OT.AddExercise "tutorials\04 - ABC" "Paris_Est" "s_exo7"
    !insertmacro OT.AddExercise "tutorials\05 - Infanterie" "Paris_Est" "s_exo8"
    !insertmacro OT.AddExercise "tutorials\06 - Genie" "Paris_Est" "s_exo9"
    !insertmacro OT.AddExercise "tutorials\07 - Artillerie" "Paris_Est" "s_exo10"
    !insertmacro OT.AddExercise "tutorials\071 - NRBC" "Paris_Est" "s_exo11"
    !insertmacro OT.AddExercise "tutorials\072 - ALAT" "Paris_Est" "s_exo12"
    !insertmacro OT.AddExercise "tutorials\073 - LOG" "Paris_Est" "s_exo13"
    !insertmacro OT.AddExercise "tutorials\08 - Fonctions Avancees" "Paris_Est" "s_exo14"
    !insertmacro OT.AddExercise "tutorials\09 - Rejeu et AAA" "Paris_Est" "s_exo15"
    !insertmacro OT.AddExercise "tutorials\10 - Preparation" "Paris_Est" "s_exo16"

SectionGroupEnd

;--------------------------------
SectionGroup "Terrains" s_ter

    !insertmacro OT.AddTerrain "Angers" "s_ter1"
    !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
    !insertmacro OT.AddTerrain "larochelle" "s_ter3"
    !insertmacro OT.AddTerrain "Cabourg" "s_ter4"
    !insertmacro OT.AddTerrain "Blois" "s_ter5"

SectionGroupEnd

;--------------------------------
Section "Documentation" s_doc
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\*.pdf"
    File /r /x ".svn" "third party"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    SetOutPath "$INSTDIR\doc\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\fr\scalped\*.pdf"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Guide Utilisateur.lnk" "$INSTDIR\doc\fr\Guide Utilisateur.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
SectionEnd

SectionGroup "Shortcuts" s_sc

    ;--------------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
    
    ;--------------------------------
    Section "Quick Launch" s_quick
        SetOutPath "$INSTDIR\applications"
        StrCmp $QUICKLAUNCH $TEMP +2
        CreateShortCut "$QUICKLAUNCH\${APP_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
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
    !insertmacro OT.CheckDependency "s_exo4" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo5" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo6" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo7" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo8" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo9" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo10" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo11" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo12" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo13" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo14" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo15" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo16" "s_ter2"
    !insertmacro OT.CheckDependency "s_exo17" "s_ter4"
    !insertmacro OT.CheckDependency "s_exo18" "s_ter5"
     
FunctionEnd
