// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "PositionEditor.h"
#include "ScopeEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: PositionEditor::PositionEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
PositionEditor::PositionEditor( ScopeEditor& scope, ISpatialReferencePtr spSpatialReference )
    : GeometryEditor( spSpatialReference )
    , scope_( scope )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::~PositionEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
PositionEditor::~PositionEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::CreateGeometry
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void PositionEditor::CreateGeometry( IFeatureBufferPtr spBuffer )
{
    IPointPtr spPoint;
    Create( spPoint, true );
    scope_.WriteGeometry( spBuffer, spPoint );
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::CreateGeometry
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void PositionEditor::CreateGeometry( IFeatureBufferPtr spBuffer, const ASN1T_CoordUTM& asnUTM )
{
    IPointPtr spPoint;
    Create( spPoint, true );
    if( UpdateCoord( spPoint, asnUTM ) )
        scope_.WriteGeometry( spBuffer, spPoint );
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void PositionEditor::Write( IFeaturePtr spFeature, const ASN1T_CoordUTM& asnUTM )
{
    IPointPtr spPoint;
    Validate( spFeature, spPoint );
    if( UpdateCoord( spPoint, asnUTM ) )
        scope_.WriteGeometry( spFeature, spPoint );
}
