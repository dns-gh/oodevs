; ------------------------------------------------------------------------------
;
; This ${File} is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef PLUGIN_NAME
    !define PLUGIN_NAME "Crossbow"
!endif
!ifndef PLUGIN_VERSION
    !define PLUGIN_VERSION "1.0"
!endif
!ifndef REQUIRED_PRODUCT_VERSION
    !define REQUIRED_PRODUCT_VERSION "${APP_VERSION_MINOR}"
!endif
!ifndef SQLDIR
    !define SQLDIR "..\sql"
!endif

!macro Plugin.AddApplications
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${SetOutPath} "$INSTDIR\applications\plugins\crossbow"
    ${File} "${RUNDIR}\plugins\crossbow\" "crossbow_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\plugins\crossbow\" "plugin.xml"
    
    ${SetOutPath} "$INSTDIR\applications\plugins\crossbow\sql"
    ${File} "${SQLDIR}\" "*.sql"
    ${File} "${SQLDIR}\" "*.bat"
!macroend
