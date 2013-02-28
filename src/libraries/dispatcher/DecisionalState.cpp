// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DecisionalState.h"
#include "Model.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DecisionalState constructor
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
DecisionalState::DecisionalState( Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalState destructor
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
DecisionalState::~DecisionalState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Clear
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void DecisionalState::Clear()
{
    for( auto it = decisionalInfos_.begin(); it != decisionalInfos_.end(); ++it )
        it->second.clear();
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void DecisionalState::Update( const sword::DecisionalState& message )
{
    decisionalInfos_[ message.key() ] = message.value();
}

// -----------------------------------------------------------------------------
// Name: DecisionalState::Send
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void DecisionalState::Send( unsigned id, ClientPublisher_ABC& publisher ) const
{
    for( auto it = decisionalInfos_.begin(); it != decisionalInfos_.end(); ++it )
    {
        client::DecisionalState message;
        model_.SetToTasker( *message().mutable_source(), id );
        message().set_key( it->first.c_str() );
        message().set_value( it->second.c_str() );
        message.Send( publisher );
    }
}
