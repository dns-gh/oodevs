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
void FolkEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgFolkGraphEdgeUpdate& asn )
{
	FolkManager::PopulationInfo	info;

	info.population_ = folk_.Filter( asn, info.containers_ );
    Write( spFeature, info );
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Write
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void FolkEditor::Write( IFeatureCursorPtr spCursor )
{	
    // $$$$ JCR 2007-08-31: Revoir tout ca...
	typedef FolkManager::T_PopulationInfoVector::const_iterator	CIT_PopulationInfoVector;
	
	IFeaturePtr spFeature;
	CIT_PopulationInfoVector it = folk_.GetPopulationInfo().begin(); 
	
	spCursor->NextFeature( &spFeature );
	while ( spFeature )
	{
		Write( spFeature, *it++ );
		spCursor->NextFeature( &spFeature );
	}
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Write
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void FolkEditor::Write( IFeaturePtr spFeature, const FolkManager::PopulationInfo& info )
{
	scope_.Write( spFeature, CComBSTR( L"Individuals" ), info.population_ );
    scope_.Write( spFeature, CComBSTR( L"Pavement" ), info.containers_[0] );
    scope_.Write( spFeature, CComBSTR( L"Road" ), info.containers_[1] );
    scope_.Write( spFeature, CComBSTR( L"Office" ), info.containers_[2] );
    scope_.Write( spFeature, CComBSTR( L"Residential" ), info.containers_[3] );
    scope_.Write( spFeature, CComBSTR( L"Shop" ), info.containers_[4] );
}
