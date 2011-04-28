; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!include "tools.nsh"
!insertmacro OT.Initialize
!ifndef PRODUCT_SUFFIX
    !define PRODUCT_SUFFIX ${EXO_PACK}
!endif

Name "${PRODUCT_NAME}"
OutFile "${DISTDIR}\${PRODUCT_NAME}_${PRODUCT_SUFFIX}_${APP_VERSION_MAJOR}.exe"

; ------------------------------------------------------------------------------
SectionGroup "Models" s_mod
    !if "${EXO_PACK}" == "defense-worldwide"
        !insertmacro OT.AddPhysicalModels "ada" "worldwide" "s_phymod1"
    !else if "${EXO_PACK}" == "defense-france"
        !insertmacro OT.AddPhysicalModels "ada" "france" "s_phymod1"
    !else if "${EXO_PACK}" == "defense-scipio"
        !insertmacro OT.AddPhysicalModels "ada" "scipio-1.9.2" "s_phymod2"
    !else if "${EXO_PACK}" == "security-worldwide"
        !insertmacro OT.AddPhysicalModels "ada" "worldwide" "s_phymod1"
        !insertmacro OT.AddPhysicalModels "ada" "security-en" "s_phymod3"
        !insertmacro OT.AddPhysicalModels "ada" "security-fr" "s_phymod3"
    !else if "${EXO_PACK}" == "security-masa"
        !insertmacro OT.AddPhysicalModels "ada" "security-fr" "s_phymod3"
    !endif

    !insertmacro OT.AddPropagation "propagation"
SectionGroupEnd    

; ------------------------------------------------------------------------------
SectionGroup "Exercises" s_exo
    !ifdef STRESS
        !insertmacro OT.AddExercise "Charge" "Cabourg" "s_exo1"
    !else if "${EXO_PACK}" == "defense-worldwide"
        ; Exercises
        !insertmacro OT.AddExercise "Egypt" "Nord egypt" "s_exo1"
        !insertmacro OT.AddExercise "Paris" "Paris_Est" "s_exo2"
        !insertmacro OT.AddExercise "Scripting demo" "test" "s_exo3"
        !insertmacro OT.AddExercise "9_cases" "Angers_x9" "s_exo4"
        !insertmacro OT.AddExercise "Musoria Border Defense" "Paris_Est" "s_exo5"
        ; Tutorials
        !insertmacro OT.AddExercise "tutorials\01 - Generalities" "Paris_Est" "s_exo6"
        !insertmacro OT.AddExercise "tutorials\02 - Gaming" "Paris_Est" "s_exo7"
        !insertmacro OT.AddExercise "tutorials\03 - Missions" "Paris_Est" "s_exo8"
        !insertmacro OT.AddExercise "tutorials\04 - Armor units" "Paris_Est" "s_exo9"
        !insertmacro OT.AddExercise "tutorials\05 - Infantry" "Paris_Est" "s_exo10"
        !insertmacro OT.AddExercise "tutorials\06 - Engineering" "Paris_Est" "s_exo11"
        !insertmacro OT.AddExercise "tutorials\07 - Artillery" "Paris_Est" "s_exo12"
        !insertmacro OT.AddExercise "tutorials\08 - Logistic" "Paris_Est" "s_exo13"
        !insertmacro OT.AddExercise "tutorials\09 - Air units" "Paris_Est" "s_exo14"
        !insertmacro OT.AddExercise "tutorials\10 - NBC" "Paris_Est" "s_exo15"
        !insertmacro OT.AddExercise "tutorials\11 - Preparation" "Paris_Est" "s_exo16"
        !insertmacro OT.AddExercise "tutorials\12 - AAR" "Paris_Est" "s_exo17"
        !insertmacro OT.AddExercise "tutorials\13 - Advanced features" "Paris_Est" "s_exo18"
    !else if "${EXO_PACK}" == "defense-france"
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
        !insertmacro OT.AddLTOExercise "SCENARIO"
                
     !else if "${EXO_PACK}" == "defense-scipio"
        ; Exercises
        ;!insertmacro OT.AddExercise "esag" "Angers" "s_exo1"
        ;!insertmacro OT.AddExercise "CENTORSEM" "Paris_Est" "s_exo2"
        ;!insertmacro OT.AddExercise "puma" "larochelle" "s_exo3"
        ;!insertmacro OT.AddExercise "Cabourg" "Cabourg" "s_exo17"
        ;!insertmacro OT.AddExercise "Ares" "Blois" "s_exo18"
        !insertmacro OT.AddExercise "Porto - 1.9.2" "Porto" "s_exo19"
        !insertmacro OT.AddExercise "BMDrosoville-1.9.2" "Drosoville" "s_exo20"
        !insertmacro OT.AddExercise "tests\test1_increment1_v1stab" "Mailly" "s_exo21"
        !insertmacro OT.AddExercise "Poseidon VA" "Poseidon" "s_exo22"
        ; Tutorials
        ;!insertmacro OT.AddExercise "tutorials\01 - Generalites" "Paris_Est" "s_exo4"
        ;!insertmacro OT.AddExercise "tutorials\02 - Jeu" "Paris_Est" "s_exo5"
        ;!insertmacro OT.AddExercise "tutorials\03 - Mission" "Paris_Est" "s_exo6"
        ;!insertmacro OT.AddExercise "tutorials\04 - ABC" "Paris_Est" "s_exo7"
        ;!insertmacro OT.AddExercise "tutorials\05 - Infanterie" "Paris_Est" "s_exo8"
        ;!insertmacro OT.AddExercise "tutorials\06 - Genie" "Paris_Est" "s_exo9"
        ;!insertmacro OT.AddExercise "tutorials\07 - Artillerie" "Paris_Est" "s_exo10"
        ;!insertmacro OT.AddExercise "tutorials\071 - NRBC" "Paris_Est" "s_exo11"
        ;!insertmacro OT.AddExercise "tutorials\072 - ALAT" "Paris_Est" "s_exo12"
        ;!insertmacro OT.AddExercise "tutorials\073 - LOG" "Paris_Est" "s_exo13"
        ;!insertmacro OT.AddExercise "tutorials\08 - Fonctions Avancees" "Paris_Est" "s_exo14"
        ;!insertmacro OT.AddExercise "tutorials\09 - Rejeu et AAA" "Paris_Est" "s_exo15"
        ;!insertmacro OT.AddExercise "tutorials\10 - Preparation" "Paris_Est" "s_exo16"
        ; Validation
        !insertmacro OT.AddExercise "exercisesVA\Poseidon VA Missions Communes" "Porto" "s_exo23"
        !insertmacro OT.AddExercise "exercisesVA\Poseidon VA pions" "Porto" "s_exo24"
        !insertmacro OT.AddExercise "exercisesVA\Poseidon VA Automates" "Porto" "s_exo25"
 
     !else if "${EXO_PACK}" == "security-worldwide"
        !insertmacro OT.AddExercise "bruxelles" "bruxelles" "s_exo1"
        !insertmacro OT.AddExercise "CODAH_JANUS" "lehavre_4.2.0" "s_exo2"        
     !else if "${EXO_PACK}" == "security-masa"        
        !insertmacro OT.AddExercise "bruxelles" "bruxelles" "s_exo1"
        !insertmacro OT.AddExercise "CODAH_JANUS" "lehavre_4.2.0" "s_exo2"        
        !insertmacro OT.AddExercise "montereycross" "montereycross" "s_exo3"
    !endif
SectionGroupEnd

; ------------------------------------------------------------------------------
SectionGroup "Terrains" s_ter
    !if "${EXO_PACK}" == "defense-worldwide"
        !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "test" "s_ter3"
        !insertmacro OT.AddTerrain "Angers_x9" "s_ter4"
    !else if "${EXO_PACK}" == "defense-france"
        !insertmacro OT.AddTerrain "Nord egypt" "s_ter1"
        !insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        !insertmacro OT.AddTerrain "Malaisie" "s_ter3"
        !insertmacro OT.AddTerrain "Cabourg" "s_ter4"
        !insertmacro OT.AddTerrain "Drosoville" "s_ter7"
        !insertmacro OT.AddTerrain "Angers_x9" "s_ter8"
        !insertmacro OT.AddTerrain "PortoV3Lite" "s_ter9"        
    !else if "${EXO_PACK}" == "defense-scipio"
        ;!insertmacro OT.AddTerrain "Angers" "s_ter1"
        ;!insertmacro OT.AddTerrain "Paris_Est" "s_ter2"
        ;!insertmacro OT.AddTerrain "larochelle" "s_ter3"
        ;!insertmacro OT.AddTerrain "Cabourg" "s_ter4"
        ;!insertmacro OT.AddTerrain "Blois" "s_ter5"
        !insertmacro OT.AddTerrain "Porto" "s_ter6"
        !insertmacro OT.AddTerrain "Drosoville" "s_ter7"
        !insertmacro OT.AddTerrain "Mailly" "s_ter8"
        !insertmacro OT.AddTerrain "PortoV3Lite" "s_ter9"      
     !else if "${EXO_PACK}" == "security-worldwide"   
        !insertmacro OT.AddTerrain "bruxelles" "s_ter1"
        !insertmacro OT.AddTerrain "lehavre_4.2.0" "s_ter2"  
      !else if "${EXO_PACK}" == "security-masa"             
        !insertmacro OT.AddTerrain "bruxelles" "s_ter1"
        !insertmacro OT.AddTerrain "lehavre_4.2.0" "s_ter2"  
        !insertmacro OT.AddTerrain "montereycross" "s_ter3"
    !endif
SectionGroupEnd

; ------------------------------------------------------------------------------
Section "Uninstall"
    !insertmacro OT.KillRunning
    !insertmacro OT.UninstallAdditionalComponent "Terrain"
    !insertmacro OT.Uninstall
SectionEnd

; ------------------------------------------------------------------------------
Function .onInit
    !insertmacro OT.CheckRunning
    !insertmacro OT.ChooseLanguage

    ; Set section names
    ;SectionSetText ${s_phymod} $(OT_SECTION_PHYSICAL_MODELS)
    SectionSetText ${s_mod} $(OT_SECTION_PHYSICAL_MODELS)
    SectionSetText ${s_exo} $(OT_SECTION_EXERCISES)
    SectionSetText ${s_ter} $(OT_SECTION_TERRAINS)
    
    !insertmacro MULTIUSER_INIT
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
    !insertmacro OT.ParseCommandLine
FunctionEnd

; ------------------------------------------------------------------------------
Function .onInstSuccess
    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

; ------------------------------------------------------------------------------
Function un.onInit
    !insertmacro MULTIUSER_UNINIT
FunctionEnd

; ------------------------------------------------------------------------------
Function .onSelChange
    !if "${EXO_PACK}" == "defense-worldwide"
        !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
        !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo3" "s_ter3"
        !insertmacro OT.CheckDependency "s_exo4" "s_ter4"
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
        !insertmacro OT.CheckDependency "s_exo17" "s_ter2"
        !insertmacro OT.CheckDependency "s_exo18" "s_ter2"
    !else if "${EXO_PACK}" == "defense-france"
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
    !else if "${EXO_PACK}" == "defense-scipio"
        ;!insertmacro OT.CheckDependency "s_exo1" "s_ter1"
        ;!insertmacro OT.CheckDependency "s_exo2" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo3" "s_ter3"
        ;!insertmacro OT.CheckDependency "s_exo4" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo5" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo6" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo7" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo8" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo9" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo10" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo11" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo12" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo13" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo14" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo15" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo16" "s_ter2"
        ;!insertmacro OT.CheckDependency "s_exo17" "s_ter4"
        ;!insertmacro OT.CheckDependency "s_exo18" "s_ter5"
        !insertmacro OT.CheckDependency "s_exo19" "s_ter6"
        !insertmacro OT.CheckDependency "s_exo20" "s_ter7"
        !insertmacro OT.CheckDependency "s_exo21" "s_ter8"
        !insertmacro OT.CheckDependency "s_exo22" "s_ter9"
        !insertmacro OT.CheckDependency "s_exo23" "s_ter9"
        !insertmacro OT.CheckDependency "s_exo24" "s_ter9"
        !insertmacro OT.CheckDependency "s_exo25" "s_ter9"
     !else if "${EXO_PACK}" == "security-worldwide"
        !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
        !insertmacro OT.CheckDependency "s_ter1" "s_pop1"    
        !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
        !insertmacro OT.CheckDependency "s_ter2" "s_pop2"
     !else if "${EXO_PACK}" == "security-masa"      
        !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
        !insertmacro OT.CheckDependency "s_ter1" "s_pop1"    
        !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
        !insertmacro OT.CheckDependency "s_ter2" "s_pop2"
        !insertmacro OT.CheckDependency "s_exo3" "s_ter3"
        !insertmacro OT.CheckDependency "s_ter3" "s_pop3"       
    !endif
FunctionEnd
