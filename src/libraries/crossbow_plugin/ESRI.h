// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma once

#ifndef __crossbow_Esri_h_
#define __crossbow_Esri_h_

#include <atlbase.h>
#include <atlcomcli.h>

#pragma warning( push )
#pragma warning( disable : 4146 4192 ) // 4278 4652 4336

#import "esri/esriSystem.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esri/esriSystemUI.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE", "IProgressDialog" )
#import "esri/esriGeometry.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esri/esriGeoDatabase.olb"       raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esri/esriDataSourcesGDB.olb"    raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )

/*
#import "\Program Files\ArcGIS\COM\esriSystem.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
#import "\Program Files\ArcGIS\COM\esriSystemUI.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "IProgressDialog")
#import "\Program Files\ArcGIS\COM\esriGeometry.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE" )
#import "\Program Files\ArcGIS\COM\esriDisplay.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
#import "\Program Files\ArcGIS\COM\esriGeoDatabase.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
#import "\Program Files\ArcGIS\COM\esriDataSourcesFile.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
#import "\Program Files\ArcGIS\COM\esriDataSourcesGDB.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE")
*/
#pragma warning( pop )

#endif // __crossbow_Esri_h_
