// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "FolkUpdater.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "QueryBuilder.h"
#include "WorkingSession.h"

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: FolkUpdater constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
FolkUpdater::FolkUpdater( Database_ABC& database, const WorkingSession& session )
    : database_ ( database )
    , updated_( 0 )
	, session_( session )
{
    UpdateQueryBuilder builder( database_.GetTableName( "Population" ) );

    builder.SetField( "individuals", (long)0 );
    builder.SetField( "road", (long)0 );
    builder.SetField( "pavement", (long)0 );
    builder.SetField( "office", (long)0 );
    builder.SetField( "shop", (long)0 );
    builder.SetField( "residential", (long)0 );
	builder.SetField( "session_id" , session_.GetId() );
    // Let previous state because of the delay : database_.Execute( builder );
}
    
// -----------------------------------------------------------------------------
// Name: FolkUpdater destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
FolkUpdater::~FolkUpdater()
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
// Name: FolkUpdater::Update
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkUpdater::Update( const ASN1T_MsgFolkCreation& msg )
{
    ::Update( activities_, msg.activities );
    ::Update( profiles_, msg.profiles );
    // $$$$ SBO 2007-09-19: container_size/edge_number
    edges_.resize( msg.edge_number );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void FolkUpdater::Update( const ASN1T_MsgFolkGraphUpdate& msg )
{
    if( edges_.size() == 0 )
        throw std::runtime_error( "Trying to update population graph before its creation." );    
    for( unsigned int i = 0; i < msg.n; ++i )
        Update( msg.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Drop
// Created: JCR 2007-11-21
// -----------------------------------------------------------------------------
void FolkUpdater::Drop()
{
    // LockedScopeEditor   lock( database_ );    
//    database_.ReleaseTable( "Population" );
}

// -----------------------------------------------------------------------------
// Name: FolkUpdater::Update
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkUpdater::Update( const ASN1T_MsgFolkGraphEdgeUpdate& msg )
{
    Edge& edge = edges_[msg.shp_oid];
    Update( edge, msg );
    if( ++updated_ >= edges_.size() )
    {
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "Population" ) );
        Commit( *table );
        updated_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FolkUpdater::Commit
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkUpdater::Commit( Table_ABC& table )
{    
    Row_ABC* row = table.Find( "", true );
    int checkflush = 0;
    for( CIT_Edges it = edges_.begin(); it != edges_.end() && row; ++it )
    {
        CommitEdge( *row, *it );
        
        if ( ++checkflush >= 500 )
        {
            table.UpdateRow( *row );
            checkflush = 0;
        }
        
        //table.UpdateRow( *row );
        row = table.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: FolkUpdater::CommitEdge
// Created: SBO 2007-09-19
// -----------------------------------------------------------------------------
void FolkUpdater::CommitEdge( Row_ABC& row, const Edge& edge )
{
    row.SetField( "Individuals", FieldVariant( (long)edge.population_ ) );
    row.SetField( "Pavement"   , FieldVariant( (long)edge.containers_[0] ) );
    row.SetField( "Road"       , FieldVariant( (long)edge.containers_[1] ) );
    row.SetField( "Office"     , FieldVariant( (long)edge.containers_[2] ) );
    row.SetField( "Residential", FieldVariant( (long)edge.containers_[3] ) );
    row.SetField( "Shop"       , FieldVariant( (long)edge.containers_[4] ) );
    row.SetField( "session_id" , FieldVariant( (long)session_.GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: FolkUpdater::Update
/* Read asn message containing population information and store appropriate 
   indiviudals
*/
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void FolkUpdater::Update( Edge& edge, const ASN1T_MsgFolkGraphEdgeUpdate& msg ) const
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
