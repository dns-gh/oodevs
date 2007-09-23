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
    IPolygonPtr spPolygon;
    ISegmentCollectionPtr spSegmentCollection;    
    IPointPtr spLast; 

    Create( spPolygon );
    spPolygon.QueryInterface( IID_ISegmentCollection, &spSegmentCollection);
    for ( int i = 0; i < asn.coordinates.n; ++i )
    {
        IPointPtr spCurrent;
        Create( spCurrent, true );
        if ( UpdateCoord( spCurrent, asn.coordinates.elem[ i ] ) && spLast != NULL )
        {
            ILinePtr spLine; 
            Create( spLine );
            spLine->PutCoords( spLast, spCurrent );            
            ISegmentPtr spSegment;
            spLine.QueryInterface( IID_ISegment, &spSegment );            
            spSegmentCollection->AddSegment( spSegment );
        }
        spLast = spCurrent;
    }
    long count;
    spSegmentCollection->get_SegmentCount( &count ); 
    if ( !FAILED( spPolygon->Close() ) )    
        scope_.WriteGeometry( spBuffer, spPolygon );
}

// -----------------------------------------------------------------------------
// Name: LineEditor::Update
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void LocationEditor::Update( IPointCollectionPtr spPointCollection, const ASN1T_CoordUTM& asnUTM )
{
    IPointPtr spPoint;
    Create( spPoint, true );
    UpdateCoord( spPoint, asnUTM );
    spPointCollection->AddPoint( spPoint );
}