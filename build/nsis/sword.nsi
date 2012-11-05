; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
;
; ------------------------------------------------------------------------------

RequestExecutionLevel admin

!include "tools.nsh"
!insertmacro OT.Initialize

Name "${PRODUCT_NAME}"
OutFile "${OUTFILE}"

; ------------------------------------------------------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO

    ;---- resources: localization ----
    SetOutPath "$INSTDIR\applications\resources\locales"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${RUNDIR}\*.qm"
    File /nonfatal /r "${RUNDIR}\*.ts"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;---- resources: documentation ----
    SetOutPath "$INSTDIR\applications\resources\help\en"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\final\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications\resources\help\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\fr\final\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;---- readme / changelog files ----
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\readme.txt" ; no language support
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;---- Application ----
    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL

    ;-- Specifique PACK RUNTIME --
    File "${OUTDIR}\release\applications\adaptation_app\*.exe"
    File "${OUTDIR}\release\applications\preparation_app\*.exe"
    File "${OUTDIR}\release\applications\dispatcher_app\*.exe"
    File "${OUTDIR}\release\applications\gaming_app\*.exe"
    File "${OUTDIR}\release\applications\simulation_app\*.exe"
    File "${OUTDIR}\release\applications\replayer_app\*.exe"
    File "${OUTDIR}\release\applications\selftraining_app\*.exe"
    File "${OUTDIR}\release\applications\package_app\*.exe"
    File "${OUTDIR}\release\applications\launcher_app\*.exe"
    File "${RUNDIR}\gradients.xml"
    File "${RUNDIR}\style.qss"
    File "${RUNDIR}\preferences.xml"
    File "${RUNDIR}\functions.xml"
    File "${RUNDIR}\service-config.xml"
    File "${RUNDIR}\log4cxx.dll"
    File /x "Qt*d.dll" /x "Qt*d4.dll" "${RUNDIR}\Qt*.dll"
    File /x "*D.dll" "${RUNDIR}\xerces*.dll"
    File /x "*D.dll" "${RUNDIR}\Xalan*.dll"
    File "${RUNDIR}\zlib1.dll"
!if ${PLATFORM} == "vc100_x64"
    File "${RUNDIR}\bugtrap-x64.dll"
!else
    File "${RUNDIR}\bugtrap.dll"
!endif
    File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\directia-${PLATFORM}-mt-4_6.dll"
    File "${RUNDIR}\launcher-${PLATFORM}-mt.dll"
    File "${RUNDIR}\lua-${PLATFORM}-mt-5_1_4.dll"
    File "${RUNDIR}\library_${PLATFORM}-mt.dll"
    File "${RUNDIR}\plugin_masalife_brain_${PLATFORM}-mt.plugin"
    File "${RUNDIR}\plugin_knowledge_${PLATFORM}-mt.plugin"
    File "${RUNDIR}\plugin_communication_${PLATFORM}-mt.plugin"
    File "${RUNDIR}\plugin_eventmanager_${PLATFORM}-mt-4_6.plugin"
    File "${RUNDIR}\plugin_motivation_${PLATFORM}-mt-4_6.plugin"
    File "${RUNDIR}\plugin_errorhandler_${PLATFORM}-mt-4_6.plugin"
    File /x "*_d.dll" "${RUNDIR}\gdal*.dll"
    File "${RUNDIR}\libexpat.dll"
    File "${RUNDIR}\libpq.dll"
    File "${RUNDIR}\ssleay32.dll"
    File "${RUNDIR}\libeay32.dll"
    File "${RUNDIR}\spatialite.dll"
    File "${RUNDIR}\geos_c.dll"
    File "${RUNDIR}\geos.dll"
    File "${RUNDIR}\proj.dll"
    File "${RUNDIR}\libiconv-2.dll"
    File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
    File "resources\*.ico"
    ;modules
    File "${RUNDIR}\*_module.dll"

    ; evaluation licence
    !ifdef EVALUATION
        File "${RUNDIR}\evaluation.lic"
    !endif
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;plugin definitions
    SetOutPath "$INSTDIR\applications\plugins"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r "${RUNDIR}\plugins\positions"
    File /r "${RUNDIR}\plugins\logistic"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"

    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_ADAPTATION).lnk" "$INSTDIR\applications\adaptation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
    ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_UNINSTALL).lnk" "${UNINST_EXE}"

    SetOutPath "$INSTDIR\installation files"
    File "Scipio-Sword_post_install.reg"
    File "Scipio-Sword_post_install.bat"

    ; VCRedist
    File "${OUTDIR}\vcredist_${PLATFORM}.exe"
    ExecWait '"vcredist_${PLATFORM}.exe" /S /NCRC'
    IfErrors 0 +3
        MessageBox MB_OK "$(OT_ABORTING_INSTALLATION_NOT_ADMIN)"
        Abort

SectionEnd

; ------------------------------------------------------------------------------
SectionGroup "Additional components"
    !insertmacro OT.AddOptionalComponent "Terrain"
    !insertmacro OT.AddMasaLifeIde
SectionGroupEnd

; ------------------------------------------------------------------------------
Section "Documentation" s_doc
    !insertmacro OT.AddDocumentation
SectionEnd

; ------------------------------------------------------------------------------
Section /o "OTPacks" s_otpacks
    !insertmacro OT.AddOTPacks
SectionEnd

; ------------------------------------------------------------------------------
SectionGroup "Shortcuts" s_sc
    ;----------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
    ;----------------------------
    Section "Quick Launch" s_quick
        SetOutPath "$INSTDIR\applications"
        StrCmp $QUICKLAUNCH $TEMP +2
        CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
SectionGroupEnd

; ------------------------------------------------------------------------------
Section "Uninstall"
    !insertmacro OT.KillRunning $(OT_ABORTING_UNINSTALLATION)
    !insertmacro OT.UninstallAdditionalComponent "Terrain"
    !insertmacro OT.RemoveMasaLifeIde
    !insertmacro OT.Uninstall
SectionEnd

; ------------------------------------------------------------------------------
Function .onInit
!if ${PLATFORM} == "vc100_x64"
    SetRegView 64
!endif

    !insertmacro OT.CheckAdmin
    !insertmacro OT.CheckRunning
    !insertmacro MULTIUSER_INIT
    !insertmacro OT.ParseCommandLine
    !insertmacro OT.KillRunning $(OT_ABORTING_INSTALLATION)
    !insertmacro OT.ChooseLanguage

    ; Set section names
    SectionSetText ${s_mod} $(OT_SECTION_MODELS)
    SectionSetText ${s_decmod} $(OT_SECTION_DECISIONAL_MODELS)
    ;SectionSetText ${s_decmodsrc} $(OT_SECTION_DECISIONAL_MODELS_SOURCES)
    SectionSetText ${s_doc} $(OT_SECTION_DOCUMENTATION)
    SectionSetText ${s_otpacks} $(OT_SECTION_OTPACKS)
    SectionSetText ${s_sc} $(OT_SECTION_SHORTCUTS)
    SectionSetText ${s_desktop} $(OT_SECTION_DESKTOP_SHORTCUT)
    SectionSetText ${s_quick} $(OT_SECTION_QUICKLAUNCH_SHORTCUT)


    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL

FunctionEnd

; ------------------------------------------------------------------------------
Function .onInstSuccess

    !insertmacro OT.AddUninstallEntry
    !insertmacro OT.AddFileAssoc
    !insertmacro OT.AddCommonConfig

    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL

FunctionEnd

; ------------------------------------------------------------------------------
Function un.onInit
!if ${PLATFORM} == "vc100_x64"
    SetRegView 64
!endif

    !insertmacro MULTIUSER_UNINIT
FunctionEnd

; ------------------------------------------------------------------------------
Function .onSelChange
FunctionEnd
