// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LimaOrder.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LimaOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::LimaOrder( const Common::MsgLimaOrder& message )
    : location_ ( message.lima().location() )
    , schedule_ ( message.horaire().data() )
{
    functions_.reserve( message.fonctions_size() );
    for( int i = 0; i < message.fonctions_size(); ++i )
        functions_.push_back( message.fonctions( i ) );
}

// -----------------------------------------------------------------------------
// Name: LimaOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::~LimaOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void LimaOrder::Send( Common::MsgLimaOrder& message ) const
{
    location_.Send( *message.mutable_lima()->mutable_location() );
    message.mutable_horaire()->set_data( schedule_ );
    for( std::vector< int >::const_iterator it = functions_.begin(); it !=  functions_.end(); ++it )
        message.add_fonctions( Common::MsgLimaOrder_Function( *it ) );
}
