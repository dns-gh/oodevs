; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!include "${PLUGIN}.nsh"

!define PRODUCT_SUITE_NAME "SWORD"
!define PRODUCT_NAME "${PLUGIN_NAME}"

!define UNINSTALL_LOG "uninstall_${PLUGIN_NAME}"

!define INSTDIR_REG_ROOT "SHELL_CONTEXT"
!define INSTDIR_REG_KEY  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

; Global variables
Var /GLOBAL INSTDATADIR
!include "tools.nsh"
!insertmacro OT.Initialize

Name "${PRODUCT_NAME}"
OutFile "${DISTDIR}\SWORD_${PLUGIN}_{APP_VERSION_MAJOR}_${PRODUCT_EVALUATION}.exe"

!include "WordFunc.nsh"
!insertmacro VersionCompare
!include "UninstallLog.nsh"

;--------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO
    !insertmacro Plugin.AddApplications
SectionEnd

Section Uninstall
    !insertmacro UninstallFromLog
    Delete "${UNINST_EXE}"
SectionEnd

;--------------------------------
Function .onInit
    !insertmacro OT.CheckRunning
    !insertmacro OT.ChooseLanguage
    
    ; Check that SWORD is installed
    ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_SUITE_NAME}" "DisplayVersion"
    StrCmp $R0 "" 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "${PRODUCT_SUITE_NAME} is not installed."
        Abort
    
    ${VersionCompare} "$R0" "${REQUIRED_PRODUCT_VERSION}" $R1
    ; R1=0 => Same version
    ; R1=1 => installed version is more recent
    ; R1=2 => Ok, perform update
    StrCmp $R1 0 +6
        StrCmp $R1 1 +3
            MessageBox MB_YESNO|MB_ICONEXCLAMATION "Installed ${PRODUCT_SUITE_NAME} version ($R0) is too old for this plugin.$\nRequired version is ${REQUIRED_PRODUCT_VERSION}.$\nDo you want to install anyway?" /SD IDYES IDYES DoInstall
            Abort
        MessageBox MB_YESNO|MB_ICONEXCLAMATION "Installed ${PRODUCT_SUITE_NAME} version ($R0) is too recent for this plugin.$\nRequired version is ${REQUIRED_PRODUCT_VERSION}.$\nDo you want to install anyway?" /SD IDYES IDYES DoInstall
        Abort
    
  DoInstall:
    ; Get SWORD installation path
    ReadRegStr $INSTDIR HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_SUITE_NAME}" "InstallLocation"
    StrCmp $INSTDIR "" 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to retrieve ${PRODUCT_SUITE_NAME} installation path."
        Abort
FunctionEnd

;--------------------------------
Function .onInstSuccess
    !insertmacro OT.AddUninstallEntry
    WriteUninstaller "${UNINST_EXE}"
FunctionEnd
