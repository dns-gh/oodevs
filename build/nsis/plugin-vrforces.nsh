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
    ${SetOutPath} "$INSTDIR\applications\plugins"
    ${SetOutPath} "$INSTDIR\applications\plugins\vrforces"
    ${File} "${RUNDIR}\plugins\vrforces\" "boost_serialization-vc80-mt-1_44.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "boost_signals-vc80-mt-1_44.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "extMtl.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "geometry.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "libFedtime1516.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "loggerControlHLA1516RT.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "makSettingsManager.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "matrixRT.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "mtlrt.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "tdbutil.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vlHLA1516RT.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vlRT.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vlutilRT.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfcontrolHLA1516.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfcoreHLA1516.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfExtObjectsHLA1516.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfExtProtocol.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfmsgs.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfMsgTransportHLA1516.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrforces_plugin-${PLATFORM}-mt.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfplan.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrftasks.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "vrfutil.dll"
    ${File} "${RUNDIR}\plugins\vrforces\" "VR-Link.xml"
    ${File} "${RUNDIR}\plugins\vrforces\" "HLA.dtd"
    ${File} "${RUNDIR}\plugins\vrforces\" "plugin.xml"
!macroend
