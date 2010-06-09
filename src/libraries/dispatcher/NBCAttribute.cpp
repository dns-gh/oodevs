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
NBCAttribute::NBCAttribute( const Common::MsgObjectAttributes& asnMsg )
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
void NBCAttribute::Update( const Common::MsgObjectAttributes& asn )
{
    if( asn.has_nbc() )
    {
        const Common::MsgObjectAttributeNBC& nbc = asn.nbc();
        danger_ = nbc.danger_level();
        agents_.clear();
        for( int i = 0; i < nbc.nbc_agents().elem_size(); ++i )
            agents_.push_back( nbc.nbc_agents().elem( i ) );
        //nbc_.CopyFrom( asn.nbc() );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCAttribute::Send( Common::MsgObjectAttributes& asn ) const
{
    Common::MsgObjectAttributeNBC& nbc = *asn.mutable_nbc();
    nbc.set_danger_level( danger_ );
    nbc.mutable_nbc_agents();
    for( std::vector< unsigned int >::const_iterator it = agents_.begin(); it != agents_.end(); ++it )
        nbc.mutable_nbc_agents()->add_elem( *it );
    //asn.mutable_nbc()->CopyFrom( nbc_ );
}
