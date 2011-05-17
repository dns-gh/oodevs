; ------------------------------------------------------------------------------
;
; This ${File} is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef PLUGIN_NAME
    !define PLUGIN_NAME "HLA"
!endif
!ifndef PLUGIN_VERSION
    !define PLUGIN_VERSION "1.0"
!endif
!ifndef REQUIRED_PRODUCT_VERSION
    !define REQUIRED_PRODUCT_VERSION "${APP_VERSION_MINOR}"
!endif

!macro Plugin.AddApplications
    ${SetOutPath} "$INSTDIR\applications"
    ${File} "${RUNDIR}\" "hla_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\" "libFedTime1516e.dll"    
    
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${File} "${RUNDIR}\plugins\" "hla.xml"
!macroend
