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

//using namespace Common;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )    
{
    nbc_.mutable_nbc_agents()->mutable_elem();
    nbc_.set_danger_level ( 0 );
    Update( asnMsg );    
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    Clear();
}  

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if ( asnMsg.has_nbc() )
    {
        Clear();
//        nbc_.mutable_nbc_agents()->set_n ( asnMsg.nbc().nbc_agents().elem_size() );
//        for( unsigned int i = 0; i < nbc_.nbc_agents().elem_size(); ++i)
//            nbc_.mutable_nbc_agents()->add_elem( nbc_.nbc_agents().elem( i ) );

        for( int i = 0; i < nbc_.nbc_agents().elem_size(); ++i)
            nbc_.mutable_nbc_agents()->add_elem( asnMsg.nbc().nbc_agents().elem( i ) );
        

//        memcpy( nbc_.nbc_agents().elem().begin(), asnMsg.nbc().nbc_agents().elem().begin(), sizeof( int ) * nbc_.nbc_agents().elem_size() );
        nbc_.set_danger_level ( asnMsg.nbc().danger_level() );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    for( int i = 0; i < nbc_.nbc_agents().elem_size(); ++i )
        asnMsg.mutable_nbc()->mutable_nbc_agents()->set_elem( i, nbc_.nbc_agents().elem( i ) ); 
    asnMsg.mutable_nbc()->set_danger_level ( nbc_.danger_level() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Clear
// Created: JCR 2008-08-27
// -----------------------------------------------------------------------------
void NBCAttribute::Clear()
{
    if ( nbc_.nbc_agents().elem_size() > 0 )
    {
        nbc_.mutable_nbc_agents()->Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void NBCAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
