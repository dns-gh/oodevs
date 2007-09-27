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
#include "Table_ABC.h"
#include "Row_ABC.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: FolkEditor constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
FolkEditor::FolkEditor()
    : updated_( 0 )
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

namespace
{
    template< typename Message >
    void Update( std::vector< std::string >& list, const Message& msg )
    {
        list.resize( msg.n );
        for( unsigned i = 0; i < msg.n; ++i )
            list[i] = (const char*)msg.elem[i].data; // $$$$ SBO 2007-09-26: length ?
    }
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Update
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkEditor::Update( const ASN1T_MsgFolkCreation& msg )
{
    ::Update( activities_, msg.activities );
    ::Update( profiles_, msg.profiles );
    // $$$$ SBO 2007-09-19: container_size/edge_number
    edges_.resize( msg.edge_number );
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Update
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkEditor::Update( Table_ABC& table, const ASN1T_MsgFolkGraphUpdate& msg )
{
    if( edges_.size() == 0 )
        throw std::runtime_error( "Trying to update population graph before its creation." );
    table.BeginTransaction();
    for( unsigned int i = 0; i < msg.n; ++i )
        Update( table, msg.elem[i] );
    table.EndTransaction();
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Update
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkEditor::Update( Table_ABC& table, const ASN1T_MsgFolkGraphEdgeUpdate& msg )
{
    Edge& edge = edges_[msg.shp_oid];
    Update( edge, msg );
    if( ++updated_ >= edges_.size() )
    {
        Commit( table );
        updated_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Commit
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkEditor::Commit( Table_ABC& table )
{
    Row_ABC* row = table.Find( "" );
    for( CIT_Edges it = edges_.begin(); it != edges_.end() && row; ++it )
    {
        CommitEdge( *row, *it );
        table.UpdateRow( *row );
        row = table.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::CommitEdge
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkEditor::CommitEdge( Row_ABC& row, const Edge& edge )
{
    row.SetField( "Individuals", FieldVariant( (long)edge.population_ ) );
    row.SetField( "Pavement"   , FieldVariant( (long)edge.containers_[0] ) );
    row.SetField( "Road"       , FieldVariant( (long)edge.containers_[1] ) );
    row.SetField( "Office"     , FieldVariant( (long)edge.containers_[2] ) );
    row.SetField( "Residential", FieldVariant( (long)edge.containers_[3] ) );
    row.SetField( "Shop"       , FieldVariant( (long)edge.containers_[4] ) );
}

// -----------------------------------------------------------------------------
// Name: FolkEditor::Update
/* Read asn message containing population information and store appropriate 
   indiviudals
*/
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void FolkEditor::Update( Edge& edge, const ASN1T_MsgFolkGraphEdgeUpdate& msg ) const
{
	const unsigned size = activities_.size() * profiles_.size();
    int c = -1;
    edge.population_ = 0;
	for ( unsigned i = 0; i < msg.population_occupation.n; ++i )
    {
        const int individuals = msg.population_occupation.elem[i];
        if ( i % size == 0 )
            ++c;
        edge.population_    += individuals;
        edge.containers_[c] += individuals;		
    }
}
