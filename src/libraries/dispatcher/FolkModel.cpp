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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FolkModel constructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkModel::FolkModel()
    : container_size( 0 )
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
void FolkModel::Update( const ASN1T_MsgFolkCreation& message )
{
    activities_.reserve( message.activities.n );
    for( unsigned i = 0; i < message.activities.n; ++i )
        activities_.push_back( (const char*)message.activities.elem[i].data );
    
    profiles_.reserve( message.profiles.n );
    for( unsigned i = 0; i < message.profiles.n; ++i )
        profiles_.push_back( (const char*)message.profiles.elem[i].data );

    container_size = message.container_size;
    edge_number    = message.edge_number;
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
        AsnMsgSimToClientFolkCreation asn;
        asn().container_size = container_size;
        asn().edge_number    = edge_number;
        std::vector< ASN1TDynOctStr > act, pro;
        for( CIT_Names it = activities_.begin(); it != activities_.end(); ++it )
            act.push_back( it->c_str() );
        for( CIT_Names it = profiles_.begin(); it != profiles_.end(); ++it )
            pro.push_back( it->c_str() );

        asn().activities.n    = act.size();
        asn().activities.elem = act.empty() ? 0 : &act.front();
        asn().profiles.n    = pro.size();
        asn().profiles.elem = pro.empty() ? 0 : &pro.front();

        asn.Send( publisher );
    }
}
