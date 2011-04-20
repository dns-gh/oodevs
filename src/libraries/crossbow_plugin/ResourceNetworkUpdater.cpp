// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ResourceNetworkUpdater.h"
#include "protocol/Protocol.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "WorkingSession_ABC.h"
#include <boost/lexical_cast.hpp>
#include <gdal/ogr_core.h>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkUpdater constructor
// Created: AME 2011-01-26
// -----------------------------------------------------------------------------
ResourceNetworkUpdater::ResourceNetworkUpdater( Workspace_ABC& workspace, long oid, int session_id, bool creation )
    : workspace_( workspace )
    , objectId_( oid )
    , sessionId_( session_id )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkUpdater destructor
// Created: AME 2011-01-26
// -----------------------------------------------------------------------------
ResourceNetworkUpdater::~ResourceNetworkUpdater()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkUpdater::Update
// Created: AME 2011-01-26
// -----------------------------------------------------------------------------
void ResourceNetworkUpdater::Update( const sword::ResourceNetwork& network )
{
    std::string resource_type, query;
    long network_id;
    if( network.has_resource() && network.resource().has_name() )
        resource_type = network.resource().name();
    else
        return;

    query = "object_id=" + boost::lexical_cast< std::string >( objectId_ ) +
            " AND session_id=" +  boost::lexical_cast< std::string >( sessionId_ ) +
            " AND resource_type='" + resource_type + "'";
    std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "resource_network" ) );
    Row_ABC* row = table->Find( query );

    if( !row  && creation_ )
    {
        row = &table->CreateRow();

        row->SetField( "session_id", FieldVariant( sessionId_ ) );
        row->SetField( "object_id", FieldVariant( static_cast< long >( objectId_ ) ) );
        row->SetField( "resource_type", FieldVariant( network.resource().name() ) );
    }
    else
        return;

    if( network.has_enabled() )
        row->SetField( "enabled", FieldVariant( network.enabled() ) );
    if( network.has_max_stock() )
        row->SetField( "max_stock", FieldVariant( static_cast< long >( network.max_stock() ) ) );
    if( network.has_stock() )
        row->SetField( "stock", FieldVariant( static_cast< long >( network.stock() ) ) );
    if( network.has_production() )
        row->SetField( "production", FieldVariant( static_cast< long >( network.production() ) ) );
    if( network.has_consumption() )
        row->SetField( "consumption", FieldVariant( static_cast< long >( network.consumption() ) ) );
    if( network.has_critical() )
        row->SetField( "critical", FieldVariant( network.critical() ) );

    if( !creation_ )
        table->UpdateRow( *row );
    else
    {
        table->InsertRow( *row );
        row = table->Find( query );
    }

    network_id = row->GetID();
    table.reset();
    UpdateResourceNetworkLinks( network, network_id );
}
// -----------------------------------------------------------------------------
// Name: ResourceNetworkUpdater::UpdateResourceNetworkLink
// Created: AME 2011-01-26
// -----------------------------------------------------------------------------
void ResourceNetworkUpdater::UpdateResourceNetworkLinks( const sword::ResourceNetwork& network, long network_id )
{
    if( network.link_size() != 0 && network_id != OGRNullFID )
    {
        for( int i = 0; i < network.link_size() ; i++ )
            UpdateResourceNetworkLink( network.link( i ), network_id );
    }
}
// -----------------------------------------------------------------------------
// Name: ResourceNetworkUpdater::UpdateResourceNetworkLink
// Created: AME 2011-01-26
// -----------------------------------------------------------------------------
void ResourceNetworkUpdater::UpdateResourceNetworkLink( const sword::ResourceNetwork_Link& link, long network_id )
{
    long target_id = 0;
    if( link.has_object() )
        target_id = static_cast< long >( link.object().id() );
    else
        return;

    std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "resource_network_link" ) );
    Row_ABC* row = table->Find( "network_id=" +         boost::lexical_cast< std::string >( network_id ) +
                               " AND target_oid =" +     boost::lexical_cast< std::string >( target_id ) );

    if( !row && creation_ )
    {
        row = &table->CreateRow();
        row->SetField( "target_oid", FieldVariant( target_id ) );
        row->SetField( "network_id", FieldVariant( static_cast< long >( network_id ) ) );
    }
    else
        return;

    if( link.has_capacity() )
        row->SetField( "capacity", FieldVariant( static_cast< long >( link.capacity() ) ) );
    if( link.has_flow() )
        row->SetField( "flow", FieldVariant( static_cast< long >( link.flow() ) ) );
    if( !creation_ )
        table->UpdateRow( *row );
    else
        table->InsertRow( *row );

}
