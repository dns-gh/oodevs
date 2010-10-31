; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
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
    !define PRODUCT_SUFFIX ${APP_PACK}
!endif

Name "${PRODUCT_NAME}"
OutFile "${DISTDIR}\${PRODUCT_NAME}_${PRODUCT_SUFFIX}_${APP_VERSION_MAJOR}${PRODUCT_EVALUATION}.exe"

!insertmacro UNATTENDED_UNINSTALL

!include "version.nsh"

;--------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO

    ;---- resources: localization ----
    SetOutPath "$INSTDIR\applications\resources\locales"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${RUNDIR}\*.qm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
 
    ;---- resources: documentation ----
    SetOutPath "$INSTDIR\applications\resources\help\en"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications\resources\help\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\fr\*.chm"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;---- readme / changelog files ----
    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${DOCDIR}\en\readme.txt" ; no language support
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;---- Application ----
    SetOutPath "$INSTDIR\applications"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    
    ;-- Specifique PACK RUNTIME --
    !if "${APP_PACK}" == "runtime"
        File "${OUTDIR}\release\applications\dispatcher_app\*.exe"
        File "${OUTDIR}\release\applications\gaming_app\*.exe"
        File "${OUTDIR}\release\applications\simulation_app\*.exe"
        File "${OUTDIR}\release\applications\replayer_app\*.exe"
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
        File /x "*D.dll" "${RUNDIR}\xerces*.dll"
        File /x "*D.dll" "${RUNDIR}\Xalan*.dll"
        File "${RUNDIR}\zlib1.dll"
        File "${RUNDIR}\bugtrap.dll"
        File "${RUNDIR}\shapelib.dll"
        File "${RUNDIR}\dispatcher-${PLATFORM}-mt.dll"
        File "${RUNDIR}\directia-${PLATFORM}-mt-4_6.dll"
        File "${RUNDIR}\lua-${PLATFORM}-mt-5_1_4.dll"
        File "${RUNDIR}\library_${PLATFORM}-mt.dll"
        File "${RUNDIR}\qsgistyle.dll"
        File "${RUNDIR}\plugin_military_base_${PLATFORM}-mt.dll"
        File "${RUNDIR}\plugin_terrain_analysis_${PLATFORM}-mt.dll"
        File "${RUNDIR}\terrain_analysis_${PLATFORM}-mt.dll"
        File "${RUNDIR}\terrain_interface_${PLATFORM}-mt.dll"
        File "${RUNDIR}\plugin_masalife_brain_${PLATFORM}-mt.plugin"
        File "${RUNDIR}\plugin_knowledge_${PLATFORM}-mt.plugin"
        File "${RUNDIR}\plugin_communication_${PLATFORM}-mt.plugin"
        File "${RUNDIR}\plugin_services_${PLATFORM}-mt.plugin"
        File "${RUNDIR}\plugin_default_engine_${PLATFORM}-mt.plugin"
        File "${RUNDIR}\plugin_eventmanager_${PLATFORM}-mt-4_6.plugin"
        File "${RUNDIR}\plugin_motivation_${PLATFORM}-mt-4_6.plugin"
    
        File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
        File "resources\*.ico"

    ;-- Specifique PACK PREPA --
    !else if "${APP_PACK}" == "prepa"
        ;File "${OUTDIR}\release\applications\selftraining_app\*.exe"
        File "${OUTDIR}\release\applications\adaptation_app\*.exe"
        File "${OUTDIR}\release\applications\preparation_app\*.exe"
        ;File "${OUTDIR}\release\applications\package_app\*.exe"

        File "${RUNDIR}\bugtrap.dll"
        File /x "qt-*d.dll" "${RUNDIR}\qt-*.dll"
        File /x "*D.dll" "${RUNDIR}\xerces*.dll"
        File /x "*D.dll" "${RUNDIR}\Xalan*.dll"
        File "${RUNDIR}\gradients.xml"
        File "${RUNDIR}\preferences.xml"
        
        File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
        File "resources\*.ico"


        ; terrain dependencies	
        File "${RUNDIR}\comerr32.dll" 
        File "${RUNDIR}\gssapi32.dll"
        File "${RUNDIR}\gdal*.dll"
        File "${RUNDIR}\gdal_SDE.dll"
        File /nonfatal "${RUNDIR}\geos_c.dll"
        File "${RUNDIR}\k5sprt32.dll"
        File "${RUNDIR}\krb5_32.dll"
        File "${RUNDIR}\libeay32.dll"
        File "${RUNDIR}\libiconv-2.dll"
        File "${RUNDIR}\libintl-8.dll"
        File "${RUNDIR}\libexpat.dll"
        File "${RUNDIR}\libpq.dll"
        File "${RUNDIR}\libxslt.dll"
        File "${RUNDIR}\msvcr71.dll"
        File "${RUNDIR}\ogr_SDE.dll"  
        ;----Missing File----    File "${RUNDIR}\proj.dll"

    ;-- Specifique PACK DEV --
    !else if "${APP_PACK}" == "dev"
    !endif
            
    ; evaluation licence
    !ifdef EVALUATION
        File "${RUNDIR}\evaluation.lic"
    !endif  
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    ;projection settings (used in crossbow)
    SetOutPath "$INSTDIR\applications\projection_data"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r "${RUNDIR}\projection_data\*"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\applications"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"

    !if "${APP_PACK}" == "runtime"
        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_SELF_TRAINING).lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    !else if "${APP_PACK}" == "prepa"
        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_ADAPTATION).lnk" "$INSTDIR\applications\adaptation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
        CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_PREPARATION).lnk" "$INSTDIR\applications\preparation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
    !else if "${APP_PACK}" == "dev"
    !endif
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

;--------------------------------
!if "${APP_PACK}" == "prepa"
SectionGroup "Additional components"
        !insertmacro OT.AddOptionalComponent "Terrain"
SectionGroupEnd
!endif

;--------------------------------
!if "${APP_PACK}" == "runtime"
SectionGroup "Models" s_mod
        !insertmacro OT.AddDecisionalModels "ada"
        ;!insertmacro OT.AddDecisionalModelSources "ada"
SectionGroupEnd
!endif


;--------------------------------
Section "Documentation" s_doc

    !insertmacro OT.AddDocumentation

SectionEnd

!if "${APP_PACK}" == "runtime"
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
!endif


;--------------------------------
Section "Uninstall"

    !insertmacro OT.KillRunning
    !if "${APP_PACK}" == "prepa"
        !insertmacro OT.UninstallAdditionalComponent "Terrain"
    !endif    
    !insertmacro OT.Uninstall

SectionEnd

;--------------------------------
Function .onInit

    !insertmacro OT.CheckRunning
    !insertmacro OT.ChooseLanguage

    ; Set section names    
    !if "${APP_PACK}" == "runtime"    
        SectionSetText ${s_mod} $(OT_SECTION_MODELS)        
        SectionSetText ${s_decmod} $(OT_SECTION_DECISIONAL_MODELS)                
        ;SectionSetText ${s_decmodsrc} $(OT_SECTION_DECISIONAL_MODELS_SOURCES)
    !endif        
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
     
FunctionEnd
