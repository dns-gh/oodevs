// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Esri_h_
#define __Esri_h_

#include <atlbase.h>
#include <atlcomcli.h>

#pragma warning( push )
#pragma warning( disable : 4146 4192 4652 4336 )

#import "esriSystem.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esriGeometry.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esriGeoDatabase.olb"       raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esriDataSourcesGDB.olb"    raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import "esriMilitaryAnalyst.olb"   raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )

//#import "esriSystemUI.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
//#import "esriFramework.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
//#import "esriLocation.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
//#import "esriDisplay.olb"           raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
//#import "esriCarto.olb"             raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE", "IColorSymbol", "IColorRampSymbol" )
//#import "esriMOLE.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE", "IColorSymbol", "IColorRampSymbol" )
//#import "esriMOLEUI.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE", "IColorSymbol", "IColorRampSymbol" )
// #import "esriMilitaryAnalystUI.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE", "IColorSymbol", "IColorRampSymbol" )

#pragma warning( pop )

#endif // __EsriConnector_h_
