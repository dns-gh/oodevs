// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "LineEditor.h"
#include "ScopeEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: LineEditor::LineEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
LineEditor::LineEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference )
    : GeometryEditor( spSpatialReference )
    , scope_( scope )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LineEditor::~LineEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
LineEditor::~LineEditor()
{
    // NOTHING
}

namespace
{
    void check_error()
    {
        IErrorInfoPtr ipError;
        BSTR          strError;

        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
    }
}


// -----------------------------------------------------------------------------
// Name: LineEditor::CreateGeometry
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void LineEditor::CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_Line& asnLine )
{
//    ILinePtr            spLine;

//    IPointPtr   spFromPoint;
//    Create( spFromPoint, true );
//    UpdateCoord( spFromPoint, asnLine.vecteur_point.elem[ 0 ] );
//
//    IPointPtr   spToPoint;
//    Create( spToPoint, true );
//    UpdateCoord( spToPoint, asnLine.vecteur_point.elem[ asnLine.vecteur_point.n - 1 ] );
//
//    Create( spLine );
//    if( FAILED( spLine->PutCoords( spFromPoint, spToPoint ) ) )
//        check_error();
    CreateGeometry( spBuffer, asnLine.coordinates );
}

// -----------------------------------------------------------------------------
// Name: LineEditor::CreateGeometry
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void LineEditor::CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_CoordUTMList& asnCoords )
{
    IPolylinePtr        spPolyline;
    IPointCollectionPtr spPointCollection;

    Create( spPolyline );
    spPolyline.QueryInterface( IID_IPointCollection, &spPointCollection );
    for( unsigned i = 0; i < asnCoords.n; ++i )
        Update( spPointCollection, asnCoords.elem[ i ] );
    scope_.WriteGeometry( spBuffer, spPolyline );
}

// -----------------------------------------------------------------------------
// Name: LineEditor::Update
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void LineEditor::Update( IPointCollectionPtr spPointCollection, const ASN1T_CoordUTM& asnUTM )
{
    IPointPtr spPoint;
    Create( spPoint, true );
    UpdateCoord( spPoint, asnUTM );
    if( FAILED( spPointCollection->AddPoint( spPoint ) ) )
        check_error();
}
