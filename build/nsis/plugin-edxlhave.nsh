; ------------------------------------------------------------------------------
;
; This ${File} is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef PLUGIN_NAME
    !define PLUGIN_NAME "EDXL-HAVE"
!endif
!ifndef PLUGIN_VERSION
    !define PLUGIN_VERSION "1.0"
!endif
!ifndef REQUIRED_PRODUCT_VERSION
    !define REQUIRED_PRODUCT_VERSION "${APP_VERSION_MINOR}"
!endif

!macro Plugin.AddApplications
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${SetOutPath} "$INSTDIR\applications\plugins\edxlhave"
    ${File} "${RUNDIR}\plugins\edxlhave\" "edxlhave_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\plugins\edxlhave\" "plugin.xml"
	${File} "${OUTDIR}\release\applications\edxlhave_app\edxlhave_app.exe" "edxlhave_app.exe"
!macroend
