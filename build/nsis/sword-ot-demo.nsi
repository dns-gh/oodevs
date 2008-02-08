; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef RUNDIR
    !define RUNDIR "..\..\run\vc71"
!endif
!ifndef DATADIR
    !define DATADIR "..\..\data"
!endif
!ifndef LIBDIR
    !define LIBDIR "..\..\lib\vc71"
!endif
!ifndef OUTDIR
    !define OUTDIR "..\..\out\vc71"
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

Name "SWORD Officer Training"
OutFile "${DISTDIR}\SWORD Officer Training - Demo.exe"
InstallDir "$PROGRAMFILES\SWORD Officer Training"
InstallDirRegKey HKLM "Software\Masa\Sword-OT" "Install_Dir"

LicenseForceSelection radiobuttons

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "sword-ot-demo") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Installer already running"
        Abort
FunctionEnd

;--------------------------------
Section "!Basic"
    SectionIn RO
    
    SetOutPath "$INSTDIR\applications"
    File "${OUTDIR}\release\applications\adaptation_app\*.exe"
    File "${OUTDIR}\release\applications\gaming_app\*.exe"
    File "${OUTDIR}\release\applications\preparation_app\*.exe"
    File "${OUTDIR}\release\applications\simulation_app\*.exe"
    File "${OUTDIR}\release\applications\replayer_app\*.exe"
    File "${OUTDIR}\release\applications\frontend_app\*.exe"
    File "${RUNDIR}\*.qm"
    File "${RUNDIR}\symbols.pak"
    File "${RUNDIR}\symbols.xml"
    File "${RUNDIR}\gradients.xml"
    File "${RUNDIR}\preferences.xml"
    File "${RUNDIR}\gradients.xml"    
    File "${RUNDIR}\DrawingTemplates.xml"
    File "${RUNDIR}\functions.xml"
    File "${RUNDIR}\*.svg"
    File "${RUNDIR}\license.dat"
    File "${RUNDIR}\TAO.dll"
    File "${RUNDIR}\TAO_CosNaming.dll"
    File "${RUNDIR}\TAO_IORTable.dll"
    File "${RUNDIR}\TAO_PortableServer.dll"
    File "${RUNDIR}\TAO_RTEvent.dll"
    File "${RUNDIR}\TAO_RTSched.dll"
    File "${RUNDIR}\TAO_Strategies.dll"
    File "${RUNDIR}\TAO_Svc_Utils.dll"
    File "${RUNDIR}\ace.dll"
    File "${RUNDIR}\asn1per.dll"
    ;File "${RUNDIR}\dbghelp.dll"
    File "${RUNDIR}\libFedTime.dll"
    File "${RUNDIR}\libRTI-NG.dll"
    File "${RUNDIR}\librtiInternalIntercept.dll"
    ;File "${RUNDIR}\msvcp71.dll"
    ;File "${RUNDIR}\msvcr71.dll"
    File "${RUNDIR}\qt-*.dll"
    File "${RUNDIR}\userDdm.dll"
    File "${RUNDIR}\xerces-c_2_7.dll"
    File "${RUNDIR}\zlib1.dll"
    File "${RUNDIR}\bugtrap.dll"
    File "${RUNDIR}\population-vc71-mt.dll"
    File "${RUNDIR}\shapelib.dll"
    File "${OUTDIR}\generation_app\*.exe"
    
    SetOutPath "$INSTDIR\data\terrains"
    File /r /x ".svn" "${DATADIR}\data\terrains\Nord egypt"
    File /r /x ".svn" "${DATADIR}\data\terrains\Paris_Est"
       
    SetOutPath "$INSTDIR\applications"
    WriteRegStr HKLM "Software\Masa\Sword-OT" "Install_Dir" "$INSTDIR"
    CreateDirectory "$SMPROGRAMS\SWORD Officer Training"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Adaptation.lnk" "$INSTDIR\applications\adaptation_app.exe"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Frontend.lnk" "$INSTDIR\applications\frontend_app.exe"
SectionEnd

Section "Decisional models"
    SectionIn RO
    SetOutPath "$INSTDIR\data\models\ada\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\*.xml"
SectionEnd    

Section "Physical models"
    SectionIn RO
    SetOutPath "$INSTDIR\data\models\ada\physical"
    File /r /x ".svn" "${DATADIR}\data\models\ada\physical\worldwide"
SectionEnd

Section "Exercises"
    SectionIn RO
    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\Egypt"
    File /r /x ".svn" "${DATADIR}\exercises\Paris"
    
    CreateDirectory "$SMPROGRAMS\SWORD Officer Training\Egypt Scenario Readme"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Egypt Scenario Readme\Readme.lnk" "$INSTDIR\exercises\Egypt\doc\Egypt scenario.txt"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Egypt Scenario Readme\Screenshot.lnk" "$INSTDIR\exercises\Egypt\doc\Egypt scenario.png"
    
    CreateDirectory "$SMPROGRAMS\SWORD Officer Training\Paris Scenario Readme"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Paris Scenario Readme\Readme.lnk" "$INSTDIR\exercises\Paris\doc\Paris scenario.txt"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Paris Scenario Readme\Screenshot.lnk" "$INSTDIR\exercises\Paris\doc\Paris scenario.png"
    
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Adaptation.lnk" "$INSTDIR\applications\adaptation_app.exe"    
SectionEnd

Section "Documentation"
    SectionIn RO
    SetOutPath "$INSTDIR\doc"
    File /r /x ".svn" "${DOCDIR}\*.pdf"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\User Guide.lnk" "$INSTDIR\doc\User Guide.pdf"
SectionEnd

;--------------------------------
Section "Uninstaller files"
    SectionIn RO
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Sword-OT-Masa" "DisplayName" "SWORD Officer Training"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Sword-OT-Masa" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Sword-OT-Masa" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Sword-OT-Masa" "NoRepair" 1
    WriteUninstaller "uninstall.exe"
    CreateShortCut "$SMPROGRAMS\SWORD Officer Training\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;--------------------------------
Section "Uninstall"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Sword-OT-Masa"
    DeleteRegKey HKLM "Software\Masa\Sword-OT"
    Delete "$INSTDIR\uninstall.exe"
    RmDir /r "$INSTDIR\applications"
    RmDir /r "$INSTDIR\data"
    RmDir /r "$INSTDIR\exercises"
    RmDir /r "$INSTDIR\doc"
    RmDir "$INSTDIR"
    Delete "$SMPROGRAMS\SWORD Officer Training\Adaptation.lnk"
    Delete "$SMPROGRAMS\SWORD Officer Training\Frontend.lnk"
    Delete "$SMPROGRAMS\SWORD Officer Training\User Guide.lnk"    
    Delete "$SMPROGRAMS\SWORD Officer Training\uninstall.lnk"    
    RmDir "$SMPROGRAMS\SWORD Officer Training"
SectionEnd
