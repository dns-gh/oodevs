; ------------------------------------------------------------------------------
;
; This ${File} is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
;
; ------------------------------------------------------------------------------

!ifndef PLUGIN_NAME
    !define PLUGIN_NAME "VrForces"
!endif
!ifndef PLUGIN_VERSION
    !define PLUGIN_VERSION "1.0"
!endif
!ifndef REQUIRED_PRODUCT_VERSION
    !define REQUIRED_PRODUCT_VERSION "${APP_VERSION_MINOR}"
!endif

!macro Plugin.AddApplications
    ${SetOutPath} "$INSTDIR\applications"
    ${File} "${RUNDIR}\" "boost_serialization-vc80-mt-1_44.dll"
    ${File} "${RUNDIR}\" "boost_signals-vc80-mt-1_44.dll"
    ${File} "${RUNDIR}\" "extMtl.dll"
    ${File} "${RUNDIR}\" "geometry.dll"
    ${File} "${RUNDIR}\" "loggerControlHLA13RT.dll"
    ${File} "${RUNDIR}\" "makSettingsManager.dll"
    ${File} "${RUNDIR}\" "matrixRT.dll"
    ${File} "${RUNDIR}\" "mtlrt.dll"
    ${File} "${RUNDIR}\" "tdbutil.dll"
    ${File} "${RUNDIR}\" "vlHLA13RT.dll"
    ${File} "${RUNDIR}\" "vlRT.dll"
    ${File} "${RUNDIR}\" "vlutilRT.dll"
    ${File} "${RUNDIR}\" "vrfcontrolHLA13.dll"
    ${File} "${RUNDIR}\" "vrfcoreHLA13.dll"
    ${File} "${RUNDIR}\" "vrfExtObjectsHLA13.dll"
    ${File} "${RUNDIR}\" "vrfExtProtocol.dll"
    ${File} "${RUNDIR}\" "vrfmsgs.dll"
    ${File} "${RUNDIR}\" "vrfMsgTransportHLA13.dll"
    ${File} "${RUNDIR}\" "vrforces_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\" "vrfplan.dll"
    ${File} "${RUNDIR}\" "vrftasks.dll"
    ${File} "${RUNDIR}\" "vrfutil.dll"
    ${File} "${RUNDIR}\" "VR-Link.fed"
    
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${File} "${RUNDIR}\plugins\" "vrforces.xml"
!macroend
