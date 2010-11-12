;------------------------------------------------------------------------------
;
; Officer Training Installation / Uninstallation helpers
;
;------------------------------------------------------------------------------

!ifndef locate::RMDirEmpty
    !include "Locate.nsh"
!endif

!include "Sections.nsh"

;------------------------------------------------------------------------------
; Defines required constants
;------------------------------------------------------------------------------
!macro OT.Initialize

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
        !define INSTDATADIR "$DOCUMENTS\${PRODUCT_NAME}"
    !endif

!macroend

;------------------------------------------------------------------------------
; Adds Decisional Models Section
;------------------------------------------------------------------------------
!macro OT.AddDecisionalModels DataSet
    Section "Decisional" s_decmod
        SetOutPath "${INSTDATADIR}\data\models\${DataSet}\decisional"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\models\${DataSet}\decisional\bms"
        File /r /x ".svn" "${DATADIR}\data\models\${DataSet}\decisional\directia5"
        File "${DATADIR}\data\models\${DataSet}\decisional\decisional.xml"
        SetOutPath "${INSTDATADIR}\data\models\${DataSet}\decisional\directia5\models\libs\base"
        File "${DATADIR}\data\models\${DataSet}\decisional\directia5\models\net.masagroup.military_Libs"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Decisional Model Sources Section
;------------------------------------------------------------------------------
!macro OT.AddDecisionalModelSources DataSet
    Section "Sources" s_decmodsrc
        SetOutPath "${INSTDATADIR}\data\models\${DataSet}\decisional"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\models\${DataSet}\decisional\Sources"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Physical Models Section
;------------------------------------------------------------------------------
!macro OT.AddPhysicalModels DataSet Localization SectionId
    Section "Physical - ${Localization}" ${SectionId}
        SetOutPath "${INSTDATADIR}\data\models\${DataSet}\physical\${Localization}"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\models\${DataSet}\physical\${Localization}\*"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Exercise Section
;------------------------------------------------------------------------------
!macro OT.AddExercise ExerciseName TerrainName SectionId
    Section "${ExerciseName} - ${TerrainName}" ${SectionId}
        SetOutPath "${INSTDATADIR}\exercises\${ExerciseName}"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /x ".svn" "${DATADIR}\exercises\${ExerciseName}\*.xml"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\scripts"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\docs"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\orders"
        File /nonfatal /r /x ".svn" "${DATADIR}\exercises\${ExerciseName}\sessions"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Terrain Section
;------------------------------------------------------------------------------
!macro OT.AddTerrain TerrainName SectionId
    Section "${TerrainName}" ${SectionId}
        ;SectionIn RO
        SetOutPath "${INSTDATADIR}\data\terrains\${TerrainName}"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\terrains\${TerrainName}\*"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Population Section
;------------------------------------------------------------------------------
!macro OT.AddPopulation PopulationName SectionId

    Section "${PopulationName}" ${SectionId}
        ;SectionIn RO
        SetOutPath "${INSTDATADIR}\data\population\${PopulationName}"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\population\${PopulationName}\*"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd

!macroend


;------------------------------------------------------------------------------
; Adds Propagation Section
;------------------------------------------------------------------------------
!macro OT.AddPropagation PropagationName
    ; Sample propagation model
    Section "${PropagationName}"
        SetOutPath "${INSTDATADIR}\data\propagations"
        !insertmacro UNINSTALL.LOG_OPEN_INSTALL
        File /r /x ".svn" "${DATADIR}\data\propagations\test"
        !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    SectionEnd
!macroend


;------------------------------------------------------------------------------
; Adds LTO Extension
;------------------------------------------------------------------------------
!macro OT.AddLTOExercise SectionName
    ; Sample LTO EXERCICE
    Section "${SectionName}"
    SetOutPath "${INSTDATADIR}\data\import_lto"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DATADIR}\tests\import_lto\*.xml"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    SectionEnd
!macroend

;------------------------------------------------------------------------------
; Adds Documentation Section
;------------------------------------------------------------------------------
!macro OT.AddDocumentation

    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /x ".svn" "${DOCDIR}\*.pdf"
    File /r /x ".svn" "third party"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}\Documentation"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\doc\en"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\en\final\AuthoringTool_User_Guide.pdf"
    File /r /x ".svn" "${DOCDIR}\en\final\Trainee_User_Guide.pdf"
    File /r /x ".svn" "${DOCDIR}\en\final\Reference_Guide.pdf"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\English"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\English\Authoring Tool User Guide.lnk" "$INSTDIR\doc\en\AuthoringTool_User_Guide.pdf"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\English\Trainee User Guide.lnk" "$INSTDIR\doc\en\Trainee_User_Guide.pdf"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\English\Reference Guide.lnk" "$INSTDIR\doc\en\Reference_Guide.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\doc\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\fr\final\Guide_de_Reference.pdf"
    File /r /x ".svn" "${DOCDIR}\fr\final\Guide_Utilisateur_Entraine.pdf"
    File /r /x ".svn" "${DOCDIR}\fr\final\Guide_Utilisateur_Outil_Adaptation.pdf"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\Français"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\Français\Guide de Reference.lnk" "$INSTDIR\doc\fr\Guide_de_Reference.pdf"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\Français\Guide Utilisateur Entraine.lnk" "$INSTDIR\doc\fr\Guide_Utilisateur_Entraine.pdf"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Documentation\Français\Guide Utilisateur Outil Adaptation.lnk" "$INSTDIR\doc\fr\Guide_Utilisateur_Outil_Adaptation.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

!macroend

;------------------------------------------------------------------------------
; Checks dependency between two sections
;------------------------------------------------------------------------------
!macro OT.CheckDependency FromSection ToSection

    !define ID ${__LINE__}

    Push $0
    SectionGetFlags ${${FromSection}} $0
    IntOp $0 $0 & ${SF_SELECTED}
    StrCmp $0 ${SF_SELECTED} 0 "notset${ID}"
        SectionGetFlags ${${ToSection}} $0
        IntOp $0 $0 | ${SF_SELECTED}
        SectionSetFlags ${${ToSection}} $0

    notset${ID}:
    Pop $0
    !undef ID

!macroend

;------------------------------------------------------------------------------
; Adds File Associations
;------------------------------------------------------------------------------
!macro OT.AddUninstallEntry

    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayVersion" "${APP_VERSION_MINOR}"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoModify" 1
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoRepair" 1

!macroend

;------------------------------------------------------------------------------
; Adds Common Application Configuration
;------------------------------------------------------------------------------
!macro OT.AddCommonConfig

    WriteRegStr ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}\${PRODUCT_NAME}\Common" "Language" "$(OT_LANG)"
    WriteRegStr ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}\${PRODUCT_NAME}\Common" "DataDirectory" "${INSTDATADIR}"

!macroend

;------------------------------------------------------------------------------
; Adds File Associations
;------------------------------------------------------------------------------
!macro OT.AddFileAssoc

    ; register .otpak extension association
    WriteRegStr HKCR ".otpak" "" "Officer Training Package"
    WriteRegStr HKCR "Officer Training Package\shell" "" "open"
    WriteRegStr HKCR "Officer Training Package\DefaultIcon" "" "$INSTDIR\applications\sword-ot.ico"
    WriteRegStr HKCR "Officer Training Package\shell\open\command" "" '$INSTDIR\applications\package_app.exe --install="%1"'

!macroend

;------------------------------------------------------------------------------
; Running Applications Utilities
;------------------------------------------------------------------------------
Function un.KillRunningApplication

    Processes::FindProcess "$9"
    StrCmp $R0 "0" notrunning
    MessageBox MB_YESNO|MB_ICONQUESTION $(OT_APPLICATION_IS_RUNNING) /SD IDYES IDYES kill
        MessageBox MB_OK $(OT_ABORTING_UNINSTALLATION)
        Abort
    kill:
        Pop $R0
        Processes::KillProcess "$9"
    notrunning:

FunctionEnd

!macro OT._KillRunning ApplicationName

    Push $9
    StrCpy $9 "${ApplicationName}"
    Call un.KillRunningApplication
    Pop $9

!macroend

;------------------------------------------------------------------------------
; Kill running OT processes
;------------------------------------------------------------------------------
!macro OT.KillRunning

    !insertmacro OT._KillRunning "simulation_app.exe"
    !insertmacro OT._KillRunning "adaptation_app.exe"
    !insertmacro OT._KillRunning "frontend_app.exe"
    !insertmacro OT._KillRunning "selftraining_app.exe"
    !insertmacro OT._KillRunning "gaming_app.exe"
    !insertmacro OT._KillRunning "preparation_app.exe"
    !insertmacro OT._KillRunning "replayer_app.exe"
    !insertmacro OT._KillRunning "detection_app.exe"
    !insertmacro OT._KillRunning "raster_app.exe"
    !insertmacro OT._KillRunning "generation_app.exe"
    !insertmacro OT._KillRunning "terrain_workshop_app.exe"

!macroend

;------------------------------------------------------------------------------
; Check for running installer
;------------------------------------------------------------------------------
!macro OT.CheckRunning

    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${PRODUCT_NAME}") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_ALREADY_RUNNING)"
        Abort

!macroend

;------------------------------------------------------------------------------
; Prompt installation language
;------------------------------------------------------------------------------
!macro OT.ChooseLanguage

    ifSilent +2 0
    Push ""
    Push ${LANG_ENGLISH}
    ifSilent +7 0
    Push English
    Push ${LANG_FRENCH}
    Push Français
    Push A ; auto count languages
    LangDLL::LangDialog "Installer Language" "Please select the language of the installer"
    Pop $LANGUAGE

!macroend


;------------------------------------------------------------------------------
; Additional Component Installation helper
;------------------------------------------------------------------------------
!macro OT.AddOptionalComponent ComponentName

    Section "${ComponentName}"
        SetOutPath "$INSTDIR\installation files"
        File "${OUTDIR}\terraintools_${PLATFORM}.zip"
        WriteRegStr ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}\${PRODUCT_NAME}\Common\Components\${ComponentName}" "RootDirectory" "$INSTDIR\${ComponentName}\applications"
        nsisunz::Unzip "$INSTDIR\installation files\terraintools_${PLATFORM}.zip" "$INSTDIR\${ComponentName}"
;        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_TN_TOOLS)\$(OT_GEN).lnk" "$INSTDIR\applications\generation_app.exe" ""  ;"$INSTDIR\applications\sword-ot.ico"
;    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_TN_TOOLS)\$(OT_WKP).lnk" "$INSTDIR\applications\terrain_workshop_app.exe" "" ;"$INSTDIR\applications\sword-ot.ico"
    SectionEnd

!macroend

;------------------------------------------------------------------------------
; Additional Component Uninstallation helper
;------------------------------------------------------------------------------
!macro OT.UninstallAdditionalComponent ComponentName

    RMDir /r $INSTDIR\${ComponentName}"
    ${locate::RMDirEmpty} "$INSTDIR\${ComponentName}" "/M=*.* /G=1 /B=1" $R1
    DeleteRegKey ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}\${PRODUCT_NAME}\Common\Components\${ComponentName}"

!macroend

;------------------------------------------------------------------------------
; Uninstallation helper
;------------------------------------------------------------------------------
!macro OT.Uninstall

    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL

    ${locate::RMDirEmpty} "$INSTDIR" "/M=*.* /G=1 /B=1" $R1
    ${locate::RMDirEmpty} "${INSTDATADIR}" "/M=*.* /G=1 /B=1" $R1
    ${locate::Unload}

    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"

    Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
    Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
    RmDir /r "$SMPROGRAMS\${PRODUCT_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
    ; TODO: prompt keep / delete preferences
    DeleteRegKey ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}\${PRODUCT_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "Software\${COMPANY_NAME}"

    ; unregister .otpak extension association
    DeleteRegKey HKCR ".otpak"

!macroend

;------------------------------------------------------------------------------
; Acrobat installation
;------------------------------------------------------------------------------
Function adobeReader
    Push $0
    ClearErrors
    ReadRegStr $0 HKCR "CLSID\{CA8A9780-280D-11CF-A24D-444553540000}" ""
    IfErrors 0 +3
        MessageBox MB_YESNO|MB_ICONQUESTION "$(OT_INSTALL_ADOBE_READER)" /SD IDNO IDNO +2
            ExecShell "open" "http://www.adobe.com/products/acrobat/readstep2.html"
    Pop $0
FunctionEnd
