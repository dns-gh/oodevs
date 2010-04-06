; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!include "AdvUninstLog.nsh"
!include "lang.nsh"
!include "tools.nsh"

!insertmacro OT.Initialize
;..................................................................................................

Name "${PRODUCT_NAME}"
!ifdef EVALUATION
    OutFile "${DISTDIR}\${PRODUCT_NAME}_${APP_VERSION_MAJOR}_eval.exe"
!else
    OutFile "${DISTDIR}\${PRODUCT_NAME}_${APP_VERSION_MAJOR}.exe"
!endif

!insertmacro UNATTENDED_UNINSTALL

!include "version.nsh"

;--------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO

    ; resources: localization
    SetOutPath "$INSTDIR\applications\resources\locales"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${RUNDIR}\*.qm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
 
    ; resources: documentation
    SetOutPath "$INSTDIR\applications\resources\help\en"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications\resources\help\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\fr\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ; readme / changelog files
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\readme.txt" ; no language support
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDIR}\release\applications\adaptation_app\*.exe"
    File "${OUTDIR}\release\applications\dispatcher_app\*.exe"
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
    File "${RUNDIR}\SSLEAY32.dll"
    File "${RUNDIR}\TAO.dll"
    File "${RUNDIR}\TAO_CosNaming.dll"
    File "${RUNDIR}\TAO_IORTable.dll"
    File "${RUNDIR}\TAO_PortableServer.dll"
    File "${RUNDIR}\TAO_RTEvent.dll"
    File "${RUNDIR}\TAO_RTSched.dll"
    File "${RUNDIR}\TAO_Strategies.dll"
    File "${RUNDIR}\TAO_Svc_Utils.dll"
    File "${RUNDIR}\ace.dll"
    File "${RUNDIR}\libFedTime.dll"
    File "${RUNDIR}\libRTI-NG.dll"
    File "${RUNDIR}\librtiInternalIntercept.dll"
    File /x "qt-*d.dll" "${RUNDIR}\qt-*.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_6.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\Xalan-C_1_10.dll"
    File "${RUNDIR}\XalanMessages_1_10.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\shapelib.dll"
    File "${RUNDIR}\population-${PLATFORM}-mt.dll"
    File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\directia-${PLATFORM}-mt-4_5.dll"
    File "${RUNDIR}\lua-${PLATFORM}-mt-5_1_4.dll"
    File "${RUNDIR}\comerr32.dll" 
    File "${RUNDIR}\k5sprt32.dll"
    File "${RUNDIR}\krb5_32.dll"
    File "${RUNDIR}\libeay32.dll"
    File "${RUNDIR}\libiconv-2.dll"
    File "${RUNDIR}\libintl-8.dll"
    File "${RUNDIR}\libexpat.dll"
    File "${RUNDIR}\libpq.dll"
    File "${RUNDIR}\libxslt.dll"
    File "${RUNDIR}\msvcr71.dll"
    File "${RUNDIR}\gssapi32.dll"
    File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
    File "resources\*.ico"
    
    ; terrain dependencies
    File "${RUNDIR}\expat_ogdi31.dll"
    File "${RUNDIR}\gdal16.dll"
    File "${RUNDIR}\gdal_SDE.dll"
    File /nonfatal "${RUNDIR}\geos_c.dll"
    File "${RUNDIR}\ogdi31.dll"
    File "${RUNDIR}\ogr_SDE.dll"
    File "${RUNDIR}\proj.dll"
    File "${RUNDIR}\rpf.dll"
    File "${RUNDIR}\vrf.dll"
    File "${RUNDIR}\zlib_ogdi31.dll"
    
    ; evaluation licence
    !ifdef EVALUATION
        File "${RUNDIR}\evaluation.lic"
    !endif
    
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_ADAPTATION).lnk" "$INSTDIR\applications\adaptation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_FRONTEND).lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_SELF_TRAINING).lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_UNINSTALL).lnk" "${UNINST_EXE}"

    !insertmacro OT.AddUninstallEntry
    !insertmacro OT.AddFileAssoc
    !insertmacro OT.AddCommonConfig

    SetOutPath $TEMP
    File "${OUTDIR}\vcredist_${PLATFORM}.exe"
    ExecWait '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"

SectionEnd

SectionGroup "Additional components"
    
    !insertmacro OT.AddOptionalComponent "Terrain"
    
SectionGroupEnd

;--------------------------------
SectionGroup "Models" s_mod

    !insertmacro OT.AddDecisionalModels "ada"
    !insertmacro OT.AddPhysicalModels "ada" "${APP_MODEL}"

SectionGroupEnd

;--------------------------------
SectionGroup "Exercises" s_exo

    !ifdef STRESS
        !insertmacro OT.AddExercise "Charge" "Cabourg" "s_exo1"
    !else if "${APP_MODEL}" == "worldwide"
        !insertmacro OT.AddExercise "Egypt" "Nord egypt" "s_exo1"
        !insertmacro OT.AddExercise "Paris" "Paris_Est" "s_exo2"
        !insertmacro OT.AddExercise "Scripting demo" "test" "s_exo3"
        !insertmacro OT.AddExercise "9_cases" "Angers_x9" "s_exo4"
        !insertmacro OT.AddExercise "Musoria Border Defense" "Paris_Est" "s_exo5"
        !insertmacro OT.AddExercise "tutorials\Generalities" "Paris_Est" "s_exo6"
    !else if "${APP_MODEL}" == "france"
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
    !endif

SectionGroupEnd

;--------------------------------
SectionGroup "Terrains" s_ter

    !if "${APP_MODEL}" == "worldwide"
        !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "test" "s_ter3"
        !insertmacro OT.AddTerrain "Angers_x9" "s_ter4"
    !else if "${APP_MODEL}" == "france"
        !insertmacro OT.AddTerrain "Angers" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "larochelle" "s_ter3"
        !insertmacro OT.AddTerrain "Cabourg" "s_ter4"
        !insertmacro OT.AddTerrain "Blois" "s_ter5"
    !endif

SectionGroupEnd

;--------------------------------
Section "Documentation" s_doc

    !insertmacro OT.AddDocumentation

SectionEnd

SectionGroup "Shortcuts" s_sc

    ;--------------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
    
    ;--------------------------------
    Section "Quick Launch" s_quick
        SetOutPath "$INSTDIR\applications"
        StrCmp $QUICKLAUNCH $TEMP +2
        CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd

SectionGroupEnd

;--------------------------------
Section "Uninstall"

    !insertmacro OT.KillRunning
    !insertmacro OT.UninstallAdditionalComponent "Terrain"
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
    
    !insertmacro MULTIUSER_INIT
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Function un.onInit
    !insertmacro MULTIUSER_UNINIT
FunctionEnd

Function .onSelChange

    !if "${APP_MODEL}" == "worldwide"
        !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
        !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo3" "s_ter3"
        !insertmacro OT.CheckDependency "s_exo4" "s_ter4"
        !insertmacro OT.CheckDependency "s_exo5" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo6" "s_ter2"
    !else if "${APP_MODEL}" == "france"
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
    !endif
     
FunctionEnd
