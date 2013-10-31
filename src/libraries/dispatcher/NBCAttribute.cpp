// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "NBCAttribute.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( const sword::ObjectAttributes& asnMsg )
    : danger_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCAttribute::Update( const sword::ObjectAttributes& asn )
{
    if( asn.has_nbc() )
    {
        const sword::ObjectAttributeNBC& nbc = asn.nbc();
        danger_ = nbc.danger_level();
        agents_.clear();
        for( int i = 0; i < nbc.nbc_agents_size(); ++i )
            agents_.push_back( nbc.nbc_agents( i ).id() );
        //nbc_.CopyFrom( asn.nbc() );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCAttribute::Send( sword::ObjectAttributes& asn ) const
{
    sword::ObjectAttributeNBC& nbc = *asn.mutable_nbc();
    nbc.set_danger_level( danger_ );
    nbc.mutable_nbc_agents();
    for( auto it = agents_.begin(); it != agents_.end(); ++it )
        nbc.add_nbc_agents()->set_id( *it );
    //asn.mutable_nbc()->CopyFrom( nbc_ );
}
