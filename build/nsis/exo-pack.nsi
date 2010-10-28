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

!ifdef EVALUATION
    !define PRODUCT_EVALUATION "_eval"
!else
    !define PRODUCT_EVALUATION ""
!endif
!ifndef PRODUCT_SUFFIX
    !define PRODUCT_SUFFIX ${APP_MODEL}_EXOPACK
!endif

Name "${PRODUCT_NAME}"
OutFile "${DISTDIR}\${PRODUCT_NAME}_${PRODUCT_SUFFIX}_${APP_VERSION_MAJOR}.exe"

!insertmacro UNATTENDED_UNINSTALL

!include "version.nsh"

;--------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO
SectionEnd

SectionGroup "Additional components"
    
    !insertmacro OT.AddOptionalComponent "Terrain"
    
SectionGroupEnd

;--------------------------------
SectionGroup "Models" s_mod

    !insertmacro OT.AddDecisionalModels "ada"
    !if "${APP_MODEL}" == "france"
        !insertmacro OT.AddPhysicalModels "ada" "france" "s_phymod1"
        !insertmacro OT.AddPhysicalModels "ada" "scipio-1.9.2" "s_phymod2"
        !insertmacro OT.AddPhysicalModels "ada" "worldwide" "s_phymod3"
    !else if "${APP_MODEL}" == "Scipio-V1.Stab"
        !insertmacro OT.AddPhysicalModels "ada" "france" "s_phymod1"
        !insertmacro OT.AddPhysicalModels "ada" "scipio-1.9.2" "s_phymod2"
    !else
        !insertmacro OT.AddPhysicalModels "ada" "${APP_MODEL}" "s_phymod1"
    !endif

    ; Sample propagation model
    Section "Propagations"
        SetOutPath "${INSTDATADIR}\data\propagations"
        File /r /x ".svn" "${DATADIR}\data\propagations\test"
    SectionEnd
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
    !else if "${APP_MODEL}" == "Scipio-V1.Stab"
        ; Exercises
        !insertmacro OT.AddExercise "esag" "Angers" "s_exo1"
        !insertmacro OT.AddExercise "CENTORSEM" "Paris_Est" "s_exo2"
        !insertmacro OT.AddExercise "puma" "larochelle" "s_exo3"
        !insertmacro OT.AddExercise "Cabourg" "Cabourg" "s_exo17"
        !insertmacro OT.AddExercise "Ares" "Blois" "s_exo18"
        !insertmacro OT.AddExercise "Porto - 1.9.2" "Porto" "s_exo19"
        !insertmacro OT.AddExercise "BMDrosoville" "Drosoville" "s_exo20"
        !insertmacro OT.AddExercise "tests\test1_increment1_v1stab" "Mailly" "s_exo21"
        !insertmacro OT.AddExercise "Poseidon VA" "Poseidon" "s_exo22"
        ; Tutorials
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
     !else if "${APP_MODEL}" == "france"
        ; Exercises
        !insertmacro OT.AddExercise "Cabourg" "Cabourg" "s_exo1"
        !insertmacro OT.AddExercise "BMDrosoville" "Drosoville" "s_exo2"
        !insertmacro OT.AddExercise "TerrainDifficile" "Malaisie" "s_exo3"
        !insertmacro OT.AddExercise "Poseidon VA" "Poseidon" "s_exo34"
        ; Tutorials
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
        ; Tests
        !insertmacro OT.AddExercise "tests\capteurs" "Angers_x9" "s_exo17"
        !insertmacro OT.AddExercise "tests\capteurs-delai" "Angers_x9" "s_exo18"
        !insertmacro OT.AddExercise "tests\nested_knowledge_groups" "Paris_Est" "s_exo19"
        !insertmacro OT.AddExercise "tests\suicide" "Paris_Est" "s_exo20"
        !insertmacro OT.AddExercise "tests\Test1_9_2" "Cabourg" "s_exo21"
        !insertmacro OT.AddExercise "tests\ValidationTests" "Angers_x9" "s_exo22"
        !insertmacro OT.AddExercise "tests\ValidationTests aux tomates" "Angers_x9" "s_exo23"
        !insertmacro OT.AddExercise "tests\ValidationTests LOG" "Angers_x9" "s_exo24"
        !insertmacro OT.AddExercise "tests\ValidationTests LOG 1_9_2" "Angers_x9" "s_exo25"
        !insertmacro OT.AddExercise "tests\Water" "Cabourg" "s_exo26"
        !insertmacro OT.AddExercise "tests\missions" "Paris_Est" "s_exo27"
        !insertmacro OT.AddExercise "tests\embrayer" "Paris_Est" "s_exo28"
        !insertmacro OT.AddExercise "tests\jamming-shoot" "Paris_Est" "s_exo29"
        !insertmacro OT.AddExercise "tests\attrition" "Paris_Est" "s_exo30"
        !insertmacro OT.AddExercise "tests\coi" "Angers_x9" "s_exo31"
        !insertmacro OT.AddExercise "tests\Meteo" "Angers_x9" "s_exo32"
        !insertmacro OT.AddExercise "tests\embrayerAuto" "Paris_Est" "s_exo33"
        ;Test import scenario
        Section "SCENARIO"
        SetOutPath "${INSTDATADIR}\data\import_lto"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File "${DATADIR}\tests\import_lto\*.xml"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        SectionEnd
    !endif

SectionGroupEnd

;--------------------------------
SectionGroup "Terrains" s_ter

    !if "${APP_MODEL}" == "worldwide"
        !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "test" "s_ter3"
        !insertmacro OT.AddTerrain "Angers_x9" "s_ter4"
    !else if "${APP_MODEL}" == "Scipio-V1.Stab"
        !insertmacro OT.AddTerrain "Angers" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "larochelle" "s_ter3"
        !insertmacro OT.AddTerrain "Cabourg" "s_ter4"
        !insertmacro OT.AddTerrain "Blois" "s_ter5"
        !insertmacro OT.AddTerrain "Porto" "s_ter6"
        !insertmacro OT.AddTerrain "Drosoville" "s_ter7"
        !insertmacro OT.AddTerrain "Mailly" "s_ter8"
        !insertmacro OT.AddTerrain "PoseidonVALite" "s_ter9"        
    !else if "${APP_MODEL}" == "france"
        !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "Malaisie" "s_ter3"
        !insertmacro OT.AddTerrain "Cabourg" "s_ter4"
        !insertmacro OT.AddTerrain "Drosoville" "s_ter7"
        !insertmacro OT.AddTerrain "Angers_x9" "s_ter8"
        !insertmacro OT.AddTerrain "PoseidonVALite" "s_ter9"        
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
        CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
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

    ;!insertmacro OT.CheckRunning
    ;!insertmacro OT.ChooseLanguage

    ; Set section names
    ;SectionSetText ${s_mod} $(OT_SECTION_MODELS)
    ;SectionSetText ${s_decmod} $(OT_SECTION_DECISIONAL_MODELS)
    ;SectionSetText ${s_decmodsrc} $(OT_SECTION_DECISIONAL_MODELS_SOURCES)
    SectionSetText ${s_phymod} $(OT_SECTION_PHYSICAL_MODELS)
    SectionSetText ${s_exo} $(OT_SECTION_EXERCISES)
    SectionSetText ${s_ter} $(OT_SECTION_TERRAINS)
    ;SectionSetText ${s_doc} $(OT_SECTION_DOCUMENTATION)
    ;SectionSetText ${s_sc} $(OT_SECTION_SHORTCUTS)
    ;SectionSetText ${s_desktop} $(OT_SECTION_DESKTOP_SHORTCUT)
    ;SectionSetText ${s_quick} $(OT_SECTION_QUICKLAUNCH_SHORTCUT)
    
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
    !else if "${APP_MODEL}" == "Scipio-V1.Stab"
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
        !insertmacro OT.CheckDependency "s_exo19" "s_ter6"
        !insertmacro OT.CheckDependency "s_exo20" "s_ter7"
        !insertmacro OT.CheckDependency "s_exo21" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo22" "s_ter9"
    !else if "${APP_MODEL}" == "france"
        !insertmacro OT.CheckDependency "s_exo1" "s_ter4"
        !insertmacro OT.CheckDependency "s_exo2" "s_ter7"
        !insertmacro OT.CheckDependency "s_exo3" "s_ter3"
        ; tutorials
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
        ; tests
        !insertmacro OT.CheckDependency "s_exo17" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo18" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo19" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo20" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo21" "s_ter4"
        !insertmacro OT.CheckDependency "s_exo22" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo23" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo24" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo25" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo26" "s_ter4"
        !insertmacro OT.CheckDependency "s_exo27" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo28" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo29" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo30" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo31" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo32" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo33" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo34" "s_ter9"
    !endif
     
FunctionEnd
