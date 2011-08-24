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
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${SetOutPath} "$INSTDIR\applications\plugins\hla"
    ${File} "${RUNDIR}\plugins\hla\" "hla_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\plugins\hla\" "hla_1516.dll"
    ${File} "${RUNDIR}\plugins\hla\" "libFedTime1516e.dll"
    ${File} "${RUNDIR}\plugins\hla\" "dis.xml"
    ${File} "${RUNDIR}\plugins\hla\" "ASI_FOM_v2.0.8_2010.xml"
    ${File} "${RUNDIR}\plugins\hla\" "protocols.xml"
    ${File} "${RUNDIR}\plugins\hla\" "plugin.xml"
!macroend
