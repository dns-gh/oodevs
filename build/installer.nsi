; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!include "MUI.nsh"

!ifndef RUNDIR
    !define RUNDIR "..\run\vc71"
!endif
!define DATADIR "..\data"
!ifndef LIBDIR
    !define LIBDIR "..\lib\vc71"
!endif
!ifndef APPLICATIONSDIR
    !define APPLICATIONSDIR "..\applications\vc71"
!endif
!ifndef OUTDIR
    !define OUTDIR "..\out\vc71"
!endif
!ifndef DISTDIR
    !define DISTDIR "..\dist"
!endif
!ifndef DOCDIR
    !define DOCDIR "..\out\documentation"
!endif

SetCompressor lzma
Name "C-Sword"
OutFile "${DISTDIR}\csword-installer.exe"
XPStyle on
InstallDir "$PROGRAMFILES\C-Sword"
InstallDirRegKey HKLM "Software\Masa\C-Sword" "Install_Dir"

;--------------------------------
;Interface Settings
  !define MUI_ABORTWARNING

;Pages
  !insertmacro MUI_PAGE_WELCOME
  ;!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH  
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
 
;Languages
  !insertmacro MUI_LANGUAGE "English"


;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "c-sword") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Installer already running"
        Abort
FunctionEnd

Section "C-Sword"
    SectionIn RO
    
    SetOutPath "$INSTDIR\applications"       
    WriteRegStr HKLM "Software\Masa\C-Sword" "Install_Dir" "$INSTDIR"
    CreateDirectory "$SMPROGRAMS\C-Sword"
    CreateShortCut "$SMPROGRAMS\C-Sword\C-Sword simulation.lnk" "$INSTDIR\applications\simulation_app.exe" "--exercise=empty"
    CreateShortCut "$SMPROGRAMS\C-Sword\C-Sword preparation.lnk" "$INSTDIR\applications\preparation_app.exe" "--exercise=empty"
    CreateShortCut "$SMPROGRAMS\C-Sword\C-Sword gaming.lnk" "$INSTDIR\applications\gaming_app.exe" "--exercise=empty"
    CreateShortCut "$SMPROGRAMS\C-Sword\C-Sword adaptation.lnk" "$INSTDIR\applications\adaptationn_app.exe"    
SectionEnd

Section "Uninstaller files"
    SectionIn RO
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "DisplayName" "C-Sword"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa" "NoRepair" 1
    WriteUninstaller "uninstall.exe"
    CreateShortCut "$SMPROGRAMS\C-Sword\uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;--------------------------------
; Uninstaller
UninstPage uninstConfirm
UninstPage instfiles

Section "Uninstall"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\C-Sword-Masa"
    DeleteRegKey HKLM "Software\Masa\C-Sword"
    Delete "$INSTDIR\uninstall.exe"
    RmDir /r "$INSTDIR\applications"
    RmDir /r "$INSTDIR\data"
    RmDir /r "$INSTDIR\exercises"
    RmDir "$INSTDIR"
    Delete "$SMPROGRAMS\C-Sword\C-Sword simulation.lnk"
    Delete "$SMPROGRAMS\C-Sword\C-Sword preparation.lnk"
    Delete "$SMPROGRAMS\C-Sword\C-Sword gaming.lnk"
    Delete "$SMPROGRAMS\C-Sword\C-Sword adaptation.lnk"    
    Delete "$SMPROGRAMS\C-Sword\uninstall.lnk"
    RmDir "$SMPROGRAMS\C-Sword"
SectionEnd