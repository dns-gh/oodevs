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

;........................................
; DECLARATION
;........................................

!define PRODUCT_NAME "SWORD"
!define COMPANY_NAME "MASA Group"

Var /GLOBAL INSTDATADIR

;........................................
; Defined from ant call
;........................................
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
; INIT
;........................................

Function initDataDirectory
	StrCpy $INSTDATADIR "$DOCUMENTS\${PRODUCT_NAME}"
FunctionEnd

!define INSTDIR_REG_ROOT "SHELL_CONTEXT"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

!define MULTIUSER_MUI
!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "Software\${COMPANY_NAME}\${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME "InstallMode"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "Software\${COMPANY_NAME}\${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "InstallDir"
!define MULTIUSER_INSTALLMODE_INSTDIR "${COMPANY_NAME}\${PRODUCT_NAME}"

!include "MultiUser.nsh"
!include "MUI.nsh"
	
!define MUI_ABORTWARNING

;........................................
; UNINSTALLER
;........................................
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
!ifndef NO_PROMPT_PROGRAM_DIRECTORY
	!define MUI_PAGE_CUSTOMFUNCTION_PRE 	   initDataDirectory
	!insertmacro MUI_PAGE_DIRECTORY
!endif
; Data directory page if needed
!ifndef NO_PROMPT_DATA_DIRECTORY
	!define MUI_PAGE_CUSTOMFUNCTION_PRE 	   initDataDirectory
	!define MUI_DIRECTORYPAGE_VARIABLE         $INSTDATADIR
	!define MUI_PAGE_HEADER_TEXT               $(OT_DATA_PAGE_HEADER_TEXT)
	!define MUI_PAGE_HEADER_SUBTEXT            $(OT_DATA_PAGE_HEADER_SUBTEXT)
	!define MUI_DIRECTORYPAGE_TEXT_DESTINATION $(OT_DATA_PAGE_TEXT_DESTINATION)
	!define MUI_DIRECTORYPAGE_TEXT_TOP         $(OT_DATA_PAGE_TEXT_TOP)
	!insertmacro MUI_PAGE_DIRECTORY
!endif
; Installation page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

;........................................
; UNINSTALLER
;........................................
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

;........................................
; LANGUAGE
;........................................
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
