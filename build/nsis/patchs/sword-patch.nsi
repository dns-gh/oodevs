; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

;..................................................................................................
!define APP_NAME "SCALPED"

;........................................
; defined from ant call
!ifndef APP_VERSION_MAJOR
    !define APP_VERSION_MAJOR "1.0.0"
!endif

!ifndef APP_VERSION_MINOR
    !define APP_VERSION_MINOR "1.0.0.0"
!endif
;........................................

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!include "AdvUninstLog.nsh"

!include "WordFunc.nsh"
!insertmacro VersionCompare

;..................................................................................................

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
    !define INSTDATADIR "$DOCUMENTS\${APP_NAME}"
!endif

Name "${APP_NAME}"
OutFile "${DISTDIR}\${APP_NAME}_${APP_VERSION_MINOR}_patch.exe"
InstallDir "$PROGRAMFILES\${APP_NAME}"
InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

!include lang.nsh
!include version.nsh

LicenseLangString LICENSE ${LANG_FRENCH} "license-scalped.txt"


!macro INSTALL.DECLARE_PATCH patchName sourceDir destinationDir
    !verbose push
    ;!verbose 4
    
    !define PATCH_SOURCE_ROOT "${sourceDir}"
    !define PATCH_FILES_ROOT "${patchName}"
    !define PATCH_INSTALL_ROOT ${destinationDir}
    
    !include "${patchName}.nsi"
    
    !undef PATCH_INSTALL_ROOT
    !undef PATCH_FILES_ROOT
    !undef PATCH_SOURCE_ROOT
    
    !verbose pop
!macroend

!macro INSTALL.PATCH patchName
    !verbose push
    ;!verbose 4
    
    Call ${patchName}.patchFilesRemoved
    Call ${patchName}.patchDirectoriesRemoved
    Call ${patchName}.patchDirectoriesAdded
    Call ${patchName}.patchFilesAdded
    Call ${patchName}.patchFilesModified
   
    !verbose pop
!macroend

!insertmacro INSTALL.DECLARE_PATCH "patch_app" "C:\Program Files\SCALPED_1.0.3" "$INSTDIR"
!insertmacro INSTALL.DECLARE_PATCH "patch_data" "C:\Documents and Settings\sbo\Mes Documents\SCALPED_1.0.3" "${INSTDATADIR}"

;--------------------------------
Section "Patch"
    SectionIn RO
    
    !insertmacro INSTALL.PATCH "patch_app"
    !insertmacro INSTALL.PATCH "patch_data"
    
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayVersion" "${APP_VERSION_MINOR}"
SectionEnd

SectionGroup "Shortcuts" s_sc

    ;--------------------------------
    Section "Desktop" s_desktop
        SetOutPath "$INSTDIR\applications"
        CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd
    
    ;--------------------------------
    Section "Quick Launch" s_quick
        SetOutPath "$INSTDIR\applications"
        StrCmp $QUICKLAUNCH $TEMP +2
        CreateShortCut "$QUICKLAUNCH\${APP_NAME}.lnk" "$INSTDIR\applications\selftraining_app.exe" "" "$INSTDIR\applications\sword-ot.ico"
    SectionEnd

SectionGroupEnd

Function un.KillRunningApplication

    Processes::FindProcess "$9"
    StrCmp $R0 "0" notrunning
    MessageBox MB_YESNO|MB_ICONQUESTION $(OT_APPLICATION_IS_RUNNING) /SD IDYES IDYES kill
        MessageBox MB_OK $(OT_ABORTING_UNINSTALLATION)
        Abort
    kill:
        Pop $0
        Processes::KillProcess "$9"
    notrunning:
    
FunctionEnd

!macro APPLICATION.KillRunning ApplicationName

    Push $9
    StrCpy $9 "${ApplicationName}"
    Call un.KillRunningApplication
    Pop $9

!macroend

!ifndef locate::RMDirEmpty
!include "Locate.nsh"
!endif

;--------------------------------
Section "Uninstall"
    !insertmacro APPLICATION.KillRunning "selftraining_app.exe"
	!insertmacro APPLICATION.KillRunning "simulation_app.exe"
	!insertmacro APPLICATION.KillRunning "gaming_app.exe"
	!insertmacro APPLICATION.KillRunning "preparation_app.exe"
	!insertmacro APPLICATION.KillRunning "replayer_app.exe"

    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL
 	
 	${locate::RMDirEmpty} "$INSTDIR" "/M=*.* /G=1 /B=1" $R1
 	${locate::RMDirEmpty} "${INSTDATADIR}" "/M=*.* /G=1 /B=1" $R1
 	${locate::Unload}
 	
    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"
    
    Delete "$DESKTOP\${APP_NAME}.lnk"
    Delete "$QUICKLAUNCH\${APP_NAME}.lnk"
    RmDir /r "$SMPROGRAMS\${APP_NAME}"
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
    
    ; unregister .otpak extension association
    DeleteRegKey HKCR ".otpak"
SectionEnd

;--------------------------------
Function .onInit
!if ${PLATFORM} == "vc100_x64"
    SetRegView 64
!endif

    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${APP_NAME}") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_ALREADY_RUNNING)"
        Abort
    Push ""
    Push ${LANG_ENGLISH}
    Push English
    Push ${LANG_FRENCH}
    Push Français
    Push A ; auto count languages
    LangDLL::LangDialog "Installer Language" "Please select the language of the installer"
    Pop $LANGUAGE

    ; Check installed
    ReadRegStr $R0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName"
    StrCmp $R0 "" 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_VERSION_NOT_INSTALLED)"
        Abort

    ; Check version
    ReadRegStr $R0 ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayVersion"
    ${VersionCompare} "$R0" "${APP_VERSION_MINOR}" $R1
    ; R1=0 => Same version
    ; R1=1 => installed version is more recent
    ; R1=2 => Ok, perform update
    StrCmp $R1 2 +6
        StrCmp $R1 1 +3
            MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_VERSION_UP_TO_DATE)"
            Abort
        MessageBox MB_OK|MB_ICONEXCLAMATION "$(OT_VERSION_MORE_RECENT)"
        Abort
    
    ; Set section names
    SectionSetText ${s_sc} $(OT_SECTION_SHORTCUTS)
    SectionSetText ${s_desktop} $(OT_SECTION_DESKTOP_SHORTCUT)
    SectionSetText ${s_quick} $(OT_SECTION_QUICKLAUNCH_SHORTCUT)
    
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd
