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
    !define DOCDIR "..\..\out\documentation"
!endif
!ifndef LIBRARIESDIR
    !define LIBRARIESDIR "..\..\src\libraries"
!endif
!ifndef APPLICATIONSDIR
    !define APPLICATIONSDIR "..\..\src\applications"
!endif

Name "C-Sword"
OutFile "${DISTDIR}\csword-installer.exe"
InstallDir "$PROGRAMFILES\C-Sword"
InstallDirRegKey HKLM "Software\Masa\C-Sword" "Install_Dir"

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "c-sword") i .r1 ?e'
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
    ;File /nonfatal "${OUTDIR}\release\applications\gaming_app\*.qm"
    ;File "${OUTDIR}\release\applications\adaptation_app\translator\*.qm"
    ;File "${LIBDIR}\clients_gui\*.qm"
    ;File "${LIBDIR}\clients_kernel\*.qm"
    ;File "${LIBDIR}\gaming\*.qm"
    ;File "${LIBDIR}\ENT\*.qm"
    ;File "${LIBDIR}\clients_gui\*.qm"
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
    File "${RUNDIR}\population-vc71-mt.dll"
    File "${RUNDIR}\shapelib.dll"
    File "${OUTDIR}\generation_app\*.exe"
    
    SetOutPath "$INSTDIR\data"
    File /r /x ".svn" "${DATADIR}\data\terrains"
       
    SetOutPath "$INSTDIR\applications"
    WriteRegStr HKLM "Software\Masa\C-Sword" "Install_Dir" "$INSTDIR"
    CreateDirectory "$SMPROGRAMS\C-Sword"
    CreateShortCut "$SMPROGRAMS\C-Sword\Adaptation.lnk" "$INSTDIR\applications\adaptation_app.exe"
    CreateShortCut "$SMPROGRAMS\C-Sword\Frontend.lnk" "$INSTDIR\applications\frontend_app.exe"
SectionEnd

;--------------------------------
SectionGroup "Main models"

Section "Decisional models"
    SetOutPath "$INSTDIR\data\models\main\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\*.xml"
SectionEnd    

Section "Decisional models sources"
    SetOutPath "$INSTDIR\data\models\main\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\Sources"
SectionEnd

Section "France Physical models"
    SetOutPath "$INSTDIR\data\models\main\physical"
    File /r /x ".svn" "${DATADIR}\data\models\main\physical\france"    
SectionEnd

Section "Worldwide Physical models"
    SetOutPath "$INSTDIR\data\models\main\physical"
    File /r /x ".svn" "${DATADIR}\data\models\main\physical\worldwide"    
SectionEnd

SectionGroupEnd

;--------------------------------
SectionGroup "ADA models"

Section "Decisional models"
    SetOutPath "$INSTDIR\data\models\ada\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\*.xml"
SectionEnd    

Section "Decisional models sources"
    SetOutPath "$INSTDIR\data\models\ada\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\ada\decisional\Sources"
SectionEnd

Section "France Physical models"
    SetOutPath "$INSTDIR\data\models\ada\physical"
    File /r /x ".svn" "${DATADIR}\data\models\ada\physical\france"    
SectionEnd

Section "Worldwide Physical models"
    SetOutPath "$INSTDIR\data\models\ada\physical"
    File /r /x ".svn" "${DATADIR}\data\models\ada\physical\worldwide"    
SectionEnd

Section "Sample exercises"
    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\ADA - Attaquer"
    File /r /x ".svn" "${DATADIR}\exercises\ADA - Donner_coup_arret"
    File /r /x ".svn" "${DATADIR}\exercises\ADA - Freiner"
    File /r /x ".svn" "${DATADIR}\exercises\BAE"
SectionEnd

SectionGroupEnd

;--------------------------------
SectionGroup "Civilian models"

Section /o "Decisional models"
    SetOutPath "$INSTDIR\data\models\civilian\decisional"
    File /nonfatal /r /x ".svn" "${DATADIR}\data\models\civilian\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\civilian\decisional\*.xml"
SectionEnd    

Section /o "Decisional models sources"
    SetOutPath "$INSTDIR\data\models\civilian\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\civilian\decisional\Sources"
SectionEnd

Section /o "Physical models"
    SetOutPath "$INSTDIR\data\models\civilian\physical"
    File /r /x ".svn" "${DATADIR}\data\models\civilian\physical"
    
    SetOutPath "$INSTDIR\data\population"
    File /r /x ".svn" "${DATADIR}\data\population"
SectionEnd

Section /o "Sample exercises"
    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\bruxelles"
SectionEnd

SectionGroupEnd

;--------------------------------
Section "Uninstaller files"
    SectionIn RO
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "DisplayName" "C-Sword"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "NoRepair" 1
    WriteUninstaller "uninstall.exe"
    CreateShortCut "$SMPROGRAMS\C-Sword\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;--------------------------------
Section "Uninstall"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa"
    DeleteRegKey HKLM "Software\Masa\C-Sword"
    Delete "$INSTDIR\uninstall.exe"
    RmDir /r "$INSTDIR\applications"
    RmDir /r "$INSTDIR\data"
    RmDir /r "$INSTDIR\exercises"
    RmDir "$INSTDIR"
    Delete "$SMPROGRAMS\C-Sword\Adaptation.lnk"
    Delete "$SMPROGRAMS\C-Sword\Frontend.lnk"
    Delete "$SMPROGRAMS\C-Sword\uninstall.lnk"
    RmDir "$SMPROGRAMS\C-Sword"
SectionEnd
