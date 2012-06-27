; ------------------------------------------------------------------------------
;
; This ${File} is part of a MASA library or program.
; Refer to the included end-user license agreement for restrictions.
;
; Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
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
    ${File} "${RUNDIR}\plugins\hla\" "libFedTime1516e.dll"
    ${File} "${RUNDIR}\plugins\hla\" "hla_1516.dll"
    ${File} "${RUNDIR}\plugins\hla\" "aggregate.xml"
    ${File} "${RUNDIR}\plugins\hla\" "vessel.xml"
    ${File} "${RUNDIR}\plugins\hla\" "component.xml"
    ${File} "${RUNDIR}\plugins\hla\" "munition.xml"
    ${File} "${RUNDIR}\plugins\hla\" "ASI_FOM_v2.0.8_2010.xml"
    ${File} "${RUNDIR}\plugins\hla\" "SWORD_SOM_ASI-2010.xml"
    ${File} "${RUNDIR}\plugins\hla\" "ASI_FOM_v2.0.8_2000.xml"
    ${File} "${RUNDIR}\plugins\hla\" "SWORD_SOM_ASI-2000.xml"
    ${File} "${RUNDIR}\plugins\hla\" "SWORD_SOM_ASI.html"
    ${File} "${RUNDIR}\plugins\hla\" "SWORD_CS.txt"
    ${File} "${RUNDIR}\plugins\hla\" "protocols.xml"
    ${File} "${RUNDIR}\plugins\hla\" "configuration.xml"
!if ${PLATFORM} == "vc80"
    ${File} "${RUNDIR}\plugins\hla\" "libFedTime.dll"
    ${File} "${RUNDIR}\plugins\hla\" "RPR2-D7.fed"
    ${File} "${RUNDIR}\plugins\hla\" "libFedTime1516.dll"
    ${FileRename} "${RUNDIR}\plugins\hla\" "plugin_vc80.xml" "plugin.xml"
!else if ${PLATFORM} == "vc100"
    ${File} "${RUNDIR}\plugins\hla\" "libFedTime1516.dll"
    ${FileRename} "${RUNDIR}\plugins\hla\" "plugin_vc100.xml" "plugin.xml"
!else 
    ${File} "${RUNDIR}\plugins\hla\" "plugin.xml"
!endif
    SetOutPath "$INSTDIR\doc\en"
    ${File} "${DOCDIR}\en\final\" "HLA_Plugin.pdf"
    SetOutPath "$INSTDIR\doc\fr"
    ${File} "${DOCDIR}\fr\final\" "HLA_Plugin.pdf"
    !insertmacro UNINSTALL.LOG_OPEN_INSTALL
    CreateShortCut "$SMPROGRAMS\${PRODUCT_SUITE_NAME}\Documentation\English\HLA Plugin.lnk" "$INSTDIR\doc\en\HLA_Plugin.pdf"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_SUITE_NAME}\Documentation\Fran�ais\HLA Plugin.lnk" "$INSTDIR\doc\fr\HLA_Plugin.pdf"
    !insertmacro UNINSTALL.LOG_CLOSE_INSTALL
!macroend
