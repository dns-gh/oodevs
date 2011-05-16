; ------------------------------------------------------------------------------
;
; This file is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------
SetCompressor lzma
XPStyle on

; ------------------------------------------------------------------------------
; Initialisation
; ------------------------------------------------------------------------------
; Global variables
Var /GLOBAL INSTDATADIR
; Base product
!define PRODUCT_NAME "SWORD"
!define COMPANY_NAME "MASA Group"

; Defined from ant call
!ifndef APP_VERSION_MAJOR
    !define APP_VERSION_MAJOR "1.0.0"
!endif
!ifndef APP_VERSION_MINOR
    !define APP_VERSION_MINOR "1.0.0.0"
!endif
; worldwide / france
!ifndef APP_MODEL
    !define APP_MODEL "worldwide"
!endif

;........................................
; Base register
!define INSTDIR_REG_ROOT "SHELL_CONTEXT"
!ifdef EXO_PACK
    !define INSTDIR_REG_KEY  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}\${EXO_PACK}"
!else
    !define INSTDIR_REG_KEY  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!endif
;........................................
; MultiUser config
!define MULTIUSER_MUI
!define MULTIUSER_EXECUTIONLEVEL                             Highest
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY          "Software\${COMPANY_NAME}\${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME    "InstallLocation"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY          "Software\${COMPANY_NAME}\${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME    "InstallLocation"
!define MULTIUSER_INSTALLMODE_INSTDIR                       "${COMPANY_NAME}\${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_FUNCTION                      OT.InitDataDirectory
!define MULTIUSER_INSTALLMODE_UNFUNCTION                    un.OT.InitRegister
;........................................
; MUI config
!define MUI_ABORTWARNING
;........................................
; Include
!include "version.nsh"
!include "MultiUser.nsh"
!include "service.nsh"
!include "MUI.nsh"
; ------------------------------------------------------------------------------
; INSTALLER
; ------------------------------------------------------------------------------
; Welcome Page
!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_WELCOME
; License page if needed
!ifdef LICENSE
    !if "${LICENSE}" != "false"
        !define MUI_LICENSEPAGE_RADIOBUTTONS
        !insertmacro MUI_PAGE_LICENSE "$(LICENSE)"
    !endif
!endif
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Multi user page
!insertmacro MULTIUSER_PAGE_INSTALLMODE
; Program directory page if needed
!ifndef EXO_PACK
    !insertmacro MUI_PAGE_DIRECTORY
!endif
; Data directory page if needed
!define MUI_DIRECTORYPAGE_VARIABLE         $INSTDATADIR
!define MUI_PAGE_HEADER_TEXT               $(OT_DATA_PAGE_HEADER_TEXT)
!define MUI_PAGE_HEADER_SUBTEXT            $(OT_DATA_PAGE_HEADER_SUBTEXT)
!define MUI_DIRECTORYPAGE_TEXT_DESTINATION $(OT_DATA_PAGE_TEXT_DESTINATION)
!define MUI_DIRECTORYPAGE_TEXT_TOP         $(OT_DATA_PAGE_TEXT_TOP)
!insertmacro MUI_PAGE_DIRECTORY
; Installation page
!insertmacro MUI_PAGE_INSTFILES
; Service installation page
Page custom ServiceInstallationDialog.Prepare ServiceInstallationDialog.Leave
; Finish page
!insertmacro MUI_PAGE_FINISH

; ------------------------------------------------------------------------------
; UNINSTALLER
; ------------------------------------------------------------------------------
; Welcome Page
!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_UNPAGE_WELCOME
; Confirm page
!insertmacro MUI_UNPAGE_CONFIRM
; Uninstallation page
!insertmacro MUI_UNPAGE_INSTFILES
; Finish page
!insertmacro MUI_UNPAGE_FINISH

; ------------------------------------------------------------------------------
; LANGUAGE
; ------------------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
