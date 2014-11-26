// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Hierarchies.h"

#include "MT_Tools/MT_Logger.h"
#include "tools/Sql.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

#include <boost/optional.hpp>

using namespace plugins;

namespace
{
    uint32_t ids = 0;

    size_t Execute( tools::Statement_ABC& st )
    {
        size_t count = 0;
        while( st.Next() )
            count++;
        return count;
    }

    void MakeTable( tools::Sql_ABC& db )
    {
        auto tr = db.Begin();
        Execute( *db.Prepare( *tr,
            "CREATE TABLE IF NOT EXISTS hierarchies ("
            "  id           INTEGER PRIMARY KEY"
            ", entity       INTEGER NOT NULL"
            ", parent       INTEGER NOT NULL"
            ", tick         INTEGER NOT NULL"
            ")" ) );
        db.Commit( *tr );
    }
}

Hierarchies::Hierarchies( const tools::Path& root,
                          const tools::Path& filename )
    : root_    ( root )
    , database_( new tools::Sql( filename ) )
    , tick_    ( 0 )
{
    MakeTable( *database_ );
}

Hierarchies::~Hierarchies()
{
    // NOTHING
}

namespace
{
    void Send( dispatcher::ClientPublisher_ABC& publisher,
        uint32_t id, uint32_t entity, uint32_t parent, uint32_t tick )
    {
        client::Hierarchy msg;
        msg().set_id( id );
        msg().set_entity( entity );
        msg().set_parent( parent );
        msg().set_tick( tick );
        msg.Send( publisher );
    }
}

namespace
{
    boost::optional< uint32_t > GetLastParent( tools::Sql_ABC& db, tools::Transaction& tr, uint32_t entity, uint32_t tick )
    {
        auto st = db.Prepare( tr,
            "SELECT parent "
            "FROM   hierarchies "
            "WHERE  entity = ? "
            "AND    tick <= ? "
            "ORDER BY tick DESC, id DESC "
            "LIMIT  1"
        );
        st->Bind( entity );
        st->Bind( tick );
        if( st->Next() )
            if( st->IsColumnDefined() )
                return st->ReadUint32();
        return boost::none;
    }
}

void Hierarchies::SetParent( uint32_t entity, uint32_t parent )
{
    try
    {
        auto tr = database_->Begin( true );
        // discard known parents
        if( auto current = GetLastParent( *database_, *tr, entity, tick_ ) )
            if( parent == *current )
                return;
        auto st = database_->Prepare( *tr,
            "REPLACE INTO hierarchies ("
            "            id "
            ",           entity "
            ",           parent "
            ",           tick "
            ") VALUES  ( ?, ?, ?, ? ) "
        );
        const auto id = ++::ids;
        st->Bind( id );
        st->Bind( entity );
        st->Bind( parent );
        st->Bind( tick_ );
        Execute( *st );
        database_->Commit( *tr );
        for( auto it = clients_.begin(); it != clients_.end(); ++it )
            Send( **it, id, entity, parent, tick_ );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

void Hierarchies::Save( const tools::Path& filename )
{
    try
    {
        database_->Save( root_ / filename );
    }
    catch( const tools::SqlException& err )
    {
        MT_LOG_ERROR_MSG( err.msg );
    }
}

void Hierarchies::SetTick( uint32_t tick )
{
    tick_ = tick;
}

void Hierarchies::SendState( dispatcher::ClientPublisher_ABC& publisher )
{
    auto tr = database_->Begin( false );
    auto st = database_->Prepare( *tr,
        "SELECT id "
        ",      entity "
        ",      parent "
        ",      tick "
        "FROM   hierarchies "
    );
    while( st->Next() )
    {
        const auto id = st->ReadUint32();
        const auto entity = st->ReadUint32();
        const auto parent = st->ReadUint32();
        const auto tick = st->ReadUint32();
        Send( publisher, id, entity, parent, tick );
    }
}

void Hierarchies::RegisterClient( dispatcher::ClientPublisher_ABC& client )
{
    clients_.insert( &client );
}

void Hierarchies::UnregisterClient( dispatcher::ClientPublisher_ABC& client )
{
    clients_.erase( &client );
}
