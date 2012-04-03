// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PerceptionMap.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PerceptionMap constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
PerceptionMap::PerceptionMap( Controller& controller, const tools::Resolver_ABC< Automat_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionMap destructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
PerceptionMap::~PerceptionMap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionMap::DoUpdate
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void PerceptionMap::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    if( message.has_perceptions() )
    {
        perceptions_.clear();
        perceptions_.reserve( message.perceptions().elem_size() );
        for( int i = 0; i < message.perceptions().elem_size(); ++i )
        {
            const Automat_ABC& agent = resolver_.Get( message.perceptions().elem( i ).automat().id() );
            // $$$$ AGE 2006-02-22:
            perceptions_.push_back( Perception( agent, (E_PerceptionResult)( 3 - message.perceptions().elem( i ).identification_level() ) ) );
        }
        controller_.Update( *this );
    }
}
