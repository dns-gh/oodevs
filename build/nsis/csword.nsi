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
    ;File /nonfatal "${OUTDIR}\release\applications\gaming_app\*.qm"
    ;File "${OUTDIR}\release\applications\adaptation_app\translator\*.qm"
    ;File "${LIBDIR}\clients_gui\*.qm"
    ;File "${LIBDIR}\clients_kernel\*.qm"
    ;File "${LIBDIR}\gaming\*.qm"
    ;File "${LIBDIR}\ENT\*.qm"
    ;File "${LIBDIR}\clients_gui\*.qm"
    File "${RUNDIR}\symbols.pak"
    File "${RUNDIR}\symbols.xml"
    File "${RUNDIR}\preferences.xml"
    File "${RUNDIR}\DrawingTemplates.xml"
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
    
    SetOutPath "$INSTDIR\data"
    File /r /x ".svn" "${DATADIR}\data\terrains"
    
    SetOutPath "$INSTDIR\data\models\main\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\Binaires"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\*.xml"
    
    SetOutPath "$INSTDIR\applications"
    WriteRegStr HKLM "Software\Masa\C-Sword" "Install_Dir" "$INSTDIR"
    CreateDirectory "$SMPROGRAMS\C-Sword"
    CreateShortCut "$SMPROGRAMS\C-Sword\Adaptation.lnk" "$INSTDIR\applications\adaptation_app.exe"
SectionEnd

;--------------------------------
Section "DirectIA models sources"
    SetOutPath "$INSTDIR\data\models\main\decisional"
    File /r /x ".svn" "${DATADIR}\data\models\main\decisional\Sources"

    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\empty-france"
SectionEnd

;--------------------------------
SectionGroup "Physical models"

Section "France"
    SetOutPath "$INSTDIR\data\models\main\physical"
    File /r /x ".svn" "${DATADIR}\data\models\main\physical\france"

    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\empty-france"

    SetOutPath "$INSTDIR\applications"
    CreateDirectory "$SMPROGRAMS\C-Sword"
    CreateDirectory "$SMPROGRAMS\C-Sword\France"
    CreateShortCut "$SMPROGRAMS\C-Sword\France\Preparation.lnk" "$INSTDIR\applications\preparation_app.exe" "--exercise=empty-france"
    CreateShortCut "$SMPROGRAMS\C-Sword\France\Simulation.lnk" "$INSTDIR\applications\simulation_app.exe" "--exercise=empty-france"
    CreateShortCut "$SMPROGRAMS\C-Sword\France\Gaming.lnk" "$INSTDIR\applications\gaming_app.exe" "--exercise=empty-france"
    CreateShortCut "$SMPROGRAMS\C-Sword\France\Replayer.lnk" "$INSTDIR\applications\replayer_app.exe" "--exercise=empty-france"    
SectionEnd

;--------------------------------
Section "Worldwide"
    SetOutPath "$INSTDIR\data\models\main\physical"
    File /r /x ".svn" "${DATADIR}\data\models\main\physical\worldwide"

    SetOutPath "$INSTDIR\exercises"
    File /r /x ".svn" "${DATADIR}\exercises\empty-worldwide"

    SetOutPath "$INSTDIR\applications"
    CreateDirectory "$SMPROGRAMS\C-Sword"
    CreateDirectory "$SMPROGRAMS\C-Sword\WorldWide"
    CreateShortCut "$SMPROGRAMS\C-Sword\WorldWide\Preparation.lnk" "$INSTDIR\applications\preparation_app.exe" "--exercise=empty-worldwide"
    CreateShortCut "$SMPROGRAMS\C-Sword\WorldWide\Simulation.lnk" "$INSTDIR\applications\simulation_app.exe" "--exercise=empty-worldwide"
    CreateShortCut "$SMPROGRAMS\C-Sword\WorldWide\Gaming.lnk" "$INSTDIR\applications\gaming_app.exe" "--exercise=empty-worldwide"
    CreateShortCut "$SMPROGRAMS\C-Sword\WorldWide\Replayer.lnk" "$INSTDIR\applications\replayer_app.exe" "--exercise=worldwide"
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
    Delete "$SMPROGRAMS\C-Sword\France\Simulation.lnk"
    Delete "$SMPROGRAMS\C-Sword\France\Preparation.lnk"
    Delete "$SMPROGRAMS\C-Sword\France\Gaming.lnk"
    Delete "$SMPROGRAMS\C-Sword\France\Replayer.lnk"
    Delete "$SMPROGRAMS\C-Sword\WorldWide\Simulation.lnk"
    Delete "$SMPROGRAMS\C-Sword\WorldWide\Preparation.lnk"
    Delete "$SMPROGRAMS\C-Sword\WorldWide\Gaming.lnk"
    Delete "$SMPROGRAMS\C-Sword\WorldWide\Replayer.lnk"
    Delete "$SMPROGRAMS\C-Sword\Adaptation.lnk"
    Delete "$SMPROGRAMS\C-Sword\uninstall.lnk"
    RmDir "$SMPROGRAMS\C-Sword\France"
    RmDir "$SMPROGRAMS\C-Sword\WorldWide"
    RmDir "$SMPROGRAMS\C-Sword"
SectionEnd
