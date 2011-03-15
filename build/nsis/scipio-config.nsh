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

;..................................................................................................
!define PRODUCT_NAME "SWORD"
!define COMPANY_NAME "MASA Group"

;........................................
; defined from ant call
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

!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES
!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE
    !if "${LICENSE}" != "false"
        !define MUI_LICENSEPAGE_RADIOBUTTONS
        !insertmacro MUI_PAGE_LICENSE "$(LICENSE)"
    !endif
!endif

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MULTIUSER_PAGE_INSTALLMODE
!insertmacro MUI_PAGE_DIRECTORY ; ABR 15.03.2001 : Fichier jamais inclu, donc pas de repertoir data ni de possibilite d'exclure le repertoire d'installation, mais peu se faire rapidement si besoin.
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
