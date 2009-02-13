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
#pragma warning( disable : 4146 4192 4278 4652 4336 )
#import <esri/esriSystem.olb>            no_registry, raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude( "OLE_COLOR", "OLE_HANDLE" )
#import <esri/esriGeometry.olb>          no_registry, raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <esri/esriGeoDatabase.olb>       no_registry, raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <esri/esriDataSourcesGDB.olb>    no_registry, raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <esri/esriDefenseSolutions.olb>  no_registry, raw_interfaces_only, raw_native_types, no_namespace, named_guids
#pragma warning( pop )

#endif // __EsriConnector_h_
