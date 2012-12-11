// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FolkModel.h"

#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FolkModel constructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkModel::FolkModel()
    : SimpleEntity<>( 0 )
    , container_size( 0 )
    , edge_number   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkModel destructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkModel::~FolkModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Update
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::Update( const sword::FolkCreation& message )
{
    activities_.reserve( message.activities().elem_size() );
    for( int i = 0; i < message.activities().elem_size(); ++i )
        activities_.push_back( message.activities().elem( i ).data() );

    profiles_.reserve( message.profiles().elem_size() );
    for( int i = 0; i < message.profiles().elem_size(); ++i )
        profiles_.push_back( message.profiles().elem( i ).data() );

    container_size = message.container_size();
    edge_number    = message.edge_number();

    ApplyUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: FolkModel::SendFullUpdate
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkModel::SendCreation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::SendCreation( ClientPublisher_ABC& publisher ) const
{
    if( container_size > 0 && edge_number > 0 )
    {
        client::FolkCreation asn;
        asn().set_container_size ( container_size );
        asn().set_edge_number    ( edge_number );
        std::vector< std::string > act, pro;
        for( auto it = activities_.begin(); it != activities_.end(); ++it )
            act.push_back( it->c_str() );
        for( auto it = profiles_.begin(); it != profiles_.end(); ++it )
            pro.push_back( it->c_str() );

        for (std::vector< std::string >::const_iterator iter(act.begin()); iter != act.end(); ++iter)
            *asn().mutable_activities()->add_elem() = *iter;
        for (std::vector< std::string >::const_iterator it(pro.begin()); it != pro.end(); ++it)
            *asn().mutable_profiles()->add_elem() = *it;

        asn.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: FolkModel::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void FolkModel::SendDestruction( ClientPublisher_ABC& ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Accept
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void FolkModel::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
