; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef BUILDDIR
    !define BUILDDIR ".."
!endif


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
    File "${OUTDIR}\release\applications\adaptation_app\adaptation_app.exe"
    File "${OUTDIR}\release\applications\gaming_app\gaming_app.exe"
    File "${OUTDIR}\release\applications\preparation_app\preparation_app.exe"
    File /oname=simulation_app.exe "${OUTDIR}\release\applications\simulation_crossbow_app\simulation_crossbow_app.exe"
    File /oname=replayer_app.exe "${OUTDIR}\release\applications\replayer_crossbow_app\replayer_crossbow_app.exe"
    File "${OUTDIR}\release\applications\frontend_app\*.exe"
    ; File "${OUTDIR}\release\applications\selftraining_app\*.exe"
    File "${OUTDIR}\release\applications\package_app\*.exe"

    SetOutPath "$INSTDIR\applications\bin"
    File /oname=dispatcher_app.exe "${OUTDIR}\release\applications\dispatcher_crossbow_app\dispatcher_crossbow_app.exe"

    SetOutPath "$INSTDIR\applications"
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
    File "${RUNDIR}\libFedTime.dll"
    File "${RUNDIR}\libRTI-NG.dll"
    File "${RUNDIR}\librtiInternalIntercept.dll"    
    File /x "msvc*d.dll" "${RUNDIR}\msvc*.dll"
    File /x "qt-*d.dll" "${RUNDIR}\qt-*.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\Xalan-C_1_10.dll"
    File "${RUNDIR}\XalanMessages_1_10.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\shapelib.dll"
    File "${RUNDIR}\population-${PLATFORM}-mt.dll"
    File "${RUNDIR}\dispatcher-crossbow-${PLATFORM}-mt.dll"
    File "${RUNDIR}\directia-${PLATFORM}-mt-4_5.dll"
    File "${RUNDIR}\lua-${PLATFORM}-mt-5_1_4.dll"
	File /x "*-gd-*" /x "*-gd.*" /x "*unittest*.*" /x "*debugger*.*" "${RUNDIR}\*.plugin"
    File /r /x ".svn" /x "*.qm" "${RUNDIR}\resources"
    File /nonfatal "${RUNDIR}\*.manifest"
    File "resources\*.ico"
    
    ; terrain dependencies
    File "${RUNDIR}\comerr32.dll" 
    File "${RUNDIR}\expat_ogdi31.dll"
    File "${RUNDIR}\gdal16.dll"
    File "${RUNDIR}\gdal_SDE.dll"
    File /nonfatal "${RUNDIR}\geos_c.dll"
    File "${RUNDIR}\gssapi32.dll"
    File "${RUNDIR}\k5sprt32.dll"
    File "${RUNDIR}\krb5_32.dll"
    File "${RUNDIR}\libeay32.dll"
    File "${RUNDIR}\libiconv-2.dll"
    File "${RUNDIR}\libintl-8.dll"
    File "${RUNDIR}\libexpat.dll"
    File "${RUNDIR}\libpq.dll"
    File "${RUNDIR}\libxslt.dll"
    File "${RUNDIR}\msvcr71.dll"
    File "${RUNDIR}\ogdi31.dll"
    File "${RUNDIR}\ogr_SDE.dll"
    File "${RUNDIR}\proj.dll"
    File "${RUNDIR}\rpf.dll"
    File "${RUNDIR}\vrf.dll"
    File "${RUNDIR}\xerces-c_2_6.dll"
    File "${RUNDIR}\zlib_ogdi31.dll"
    File "${RUNDIR}\library-vc80-mt.dll"
    
    ; evaluation licence
    !ifdef EVALUATION
        File "${RUNDIR}\evaluation.lic"
    !endif
    
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
      
    
    ; resources: sql deployment
    SetOutPath "$INSTDIR\applications\resources\sql"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${BUILDDIR}\sql\*.sql"
    File "${BUILDDIR}\sql\*.bat"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    
    SetOutPath "$INSTDIR\applications"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_ADAPTATION).lnk" "$INSTDIR\applications\adaptation_app.exe" "" "$INSTDIR\applications\adaptation.ico"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_FRONTEND).lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    ; CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_SELF_TRAINING).lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\$(OT_UNINSTALL).lnk" "${UNINST_EXE}"

    !insertmacro OT.AddUninstallEntry
    !insertmacro OT.AddFileAssoc
    !insertmacro OT.AddCommonConfig
        
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
    !insertmacro OT.AddExercise "bruxelles" "bruxelles" "s_exo1"
    !insertmacro OT.AddExercise "lehavre" "lehavre" "s_exo2"
    ;!insertmacro OT.AddExercise "montereycross" "montereycross" "s_exo3"
SectionGroupEnd

;--------------------------------
SectionGroup "Terrains" s_ter

    !insertmacro OT.AddTerrain "bruxelles" "s_ter1"
    !insertmacro OT.AddTerrain "lehavre" "s_ter2"
    ;!insertmacro OT.AddTerrain "montereycross" "s_ter3"

SectionGroupEnd

;--------------------------------
SectionGroup "Populations" s_pop

    !insertmacro OT.AddPopulation "bruxelles" "s_pop1"
    !insertmacro OT.AddPopulation "lehavre" "s_pop2"
    ;!insertmacro OT.AddPopulation "montereycross" "s_pop3"

SectionGroupEnd

;--------------------------------
Section "Documentation" s_doc

    SetOutPath "$INSTDIR\doc"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\*.pdf"
    File /r /x ".svn" "third party"
    CreateDirectory "$SMPROGRAMS\${APP_NAME}\Documentation"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath "$INSTDIR\doc\en"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\en\Trainee_User_Guide.pdf"
    File /r /x ".svn" "${DOCDIR}\en\Trainer_User_Guide.pdf"
    File /r /x ".svn" "${DOCDIR}\en\Reference_Guide.pdf"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Documentation\Trainee Guide.lnk" "$INSTDIR\doc\en\Trainee_User_Guide.pdf"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Documentation\Trainer Guide.lnk" "$INSTDIR\doc\en\Trainer_User_Guide.pdf"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Documentation\Reference Guide.lnk" "$INSTDIR\doc\en\Reference_Guide.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
    SetOutPath "$INSTDIR\doc\fr"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${DOCDIR}\fr\Guide Utilisateur.pdf"
    CreateShortCut "$SMPROGRAMS\${APP_NAME}\Documentation\Guide Utilisateur.lnk" "$INSTDIR\doc\fr\Guide Utilisateur.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
    
SectionEnd

SectionGroup "Shortcuts" s_sc

    ;--------------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\applications\frontend_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
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
    SectionSetText ${s_pop} $(OT_SECTION_POPULATIONS)
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

    !insertmacro OT.CheckDependency "s_exo1" "s_ter1"
    !insertmacro OT.CheckDependency "s_ter1" "s_pop1"
    
    !insertmacro OT.CheckDependency "s_exo2" "s_ter2"
    !insertmacro OT.CheckDependency "s_ter2" "s_pop2"
    
    ;!insertmacro OT.CheckDependency "s_exo3" "s_ter3"
    ;!insertmacro OT.CheckDependency "s_ter3" "s_pop3"
     
FunctionEnd
