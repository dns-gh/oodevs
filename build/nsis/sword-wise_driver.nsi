; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!define PRODUCT_NAME "SWORD-WISE Driver"
!define COMPANY_NAME "MASA Group"
!define WISE_VERSION "WISE Connectivity 3.9"

;........................................
; defined from ant call
!ifndef APP_VERSION_MAJOR
    !define APP_VERSION_MAJOR "1.0.0"
!endif

!ifndef APP_VERSION_MINOR
    !define APP_VERSION_MINOR "1.0.0.0"
!endif

!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANY_NAME}\${PRODUCT_NAME}"

!define UNINSTALL_LOG "SWORD_Driver_Uninstall"
!include "AdvUninstLog.nsh"

!ifndef PLATFORM
    !define PLATFORM "vc80"
!endif
!ifndef SRCDIR
    !define SRCDIR "..\..\src"
!endif
!ifndef RUNDIR
    !define RUNDIR "..\..\run\${PLATFORM}"
!endif
!ifndef OUTDIR
    !define OUTDIR "..\..\out\${PLATFORM}"
!endif
!ifndef DISTDIR
    !define DISTDIR "."
!endif

Name "${PRODUCT_NAME}"
OutFile "${DISTDIR}\${PRODUCT_NAME}_${PLATFORM}_${APP_VERSION_MAJOR}.exe"

InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir"

!insertmacro UNATTENDED_UNINSTALL

;--------------------------------
Section "!${PRODUCT_NAME}"
    SectionIn RO

    SetOutPath "$INSTDIR\Storage"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File /r /x ".svn" "${SRCDIR}\libraries\wise_driver_dll\Storage\*"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
   
    SetOutPath "$INSTDIR\bin"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    File "${OUTDIR}\release\libraries\wise_driver_dll\*.dll"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

    SetOutPath $TEMP
    File "${OUTDIR}\vcredist_${PLATFORM}.exe"
    ExecWait '"vcredist_${PLATFORM}.exe" /S /NCRC'
    Delete "vcredist_${PLATFORM}.exe"
SectionEnd

;--------------------------------
Section "Uninstall"
    !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
    !insertmacro UNINSTALL.LOG_UNINSTALL_ALL
    !insertmacro UNINSTALL.LOG_END_UNINSTALL

    Delete "${UNINST_DAT}"
    Delete "${UNINST_EXE}"
    
    DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
SectionEnd

;--------------------------------
Function .onInit
    System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${PRODUCT_NAME}") i .r1 ?e'
    
    ; Check that WISE is installed
    ReadRegStr $R0 HKLM "Software\Saab\WISE\${WISE_VERSION}\WISECore" "Installed"
    StrCmp $R0 "" 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "${WISE_VERSION} is not installed."
        Abort
    
    ; Get WISE installation path
    ReadRegStr $INSTDIR HKLM "Software\Saab\WISE\${WISE_VERSION}" "InstallPath"
    StrCmp $INSTDIR "" 0 +3
        MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to retrieve WISE installation path."
        Abort
        
    !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

;--------------------------------
Function .onInstSuccess
    ; Add uninstall information == tools.nsh -> OT.AddUninstallEntry
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallLocation" "$INSTDIR"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayVersion" "${APP_VERSION_MINOR}"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
    WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "Publisher" "${COMPANY_NAME}"
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoModify" 1
    WriteRegDWORD ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "NoRepair" 1

    ;create/update log always within .onInstSuccess function
    !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd
