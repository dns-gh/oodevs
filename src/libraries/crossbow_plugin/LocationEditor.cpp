// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "LocationEditor.h"
#include "ScopeEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: LocationEditor::LocationEditor
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
LocationEditor::LocationEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference )
    : GeometryEditor( spSpatialReference )
    , scope_ ( scope )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditor::~LocationEditor
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
LocationEditor::~LocationEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditor::CreateGeometry
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void LocationEditor::CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_Location& asn )
{
    IPointCollectionPtr spGeometry;
    
    spGeometry.CreateInstance( CLSID_Ring );
    for ( int i = 0; i < asn.coordinates.n; ++i )
    {
        IPointPtr spPoint;
        Create( spPoint, true );
        if( UpdateCoord( spPoint, asn.coordinates.elem[ i ] ) )
            spGeometry->AddPoint( spPoint );
    }
    long size = 0;
    spGeometry->get_PointCount( &size );
    if ( size > 0 )
        scope_.WriteGeometry( spBuffer, spGeometry );
}
