// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "FolkEditor.h"
#include "FolkManager.h"
#include "ScopeEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: FolkEditor constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
FolkEditor::FolkEditor( ScopeEditor& scope, const FolkManager& folk )
    : folk_ ( folk )
    , scope_ ( scope )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FolkEditor destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
FolkEditor::~FolkEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Write
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void FolkEditor::Write( IFeatureBufferPtr spFeature, const ASN1T_MsgFolkGraphEdgeUpdate& asn )
{
    std::vector<unsigned>   container( 5, 0 );
    const unsigned population = folk_.Filter( asn, container );
    
    scope_.Write( spFeature, CComBSTR( L"Individuals" ), population );
    scope_.Write( spFeature, CComBSTR( L"Pavement" ), container[0] );
    scope_.Write( spFeature, CComBSTR( L"Road" ), container[1] );
    scope_.Write( spFeature, CComBSTR( L"Office" ), container[2] );
    scope_.Write( spFeature, CComBSTR( L"Residential" ), container[3] );
    scope_.Write( spFeature, CComBSTR( L"Shop" ), container[4] );
}
