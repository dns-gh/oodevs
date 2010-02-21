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
void PerceptionMap::DoUpdate( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message )
{
    if( message.has_perception_par_compagnie()  )
    {
        perceptions_.clear();
        perceptions_.reserve( message.perception_par_compagnie().elem_size() );
        for( int i = 0; i < message.perception_par_compagnie().elem_size(); ++i )
        {
            const Automat_ABC& agent = resolver_.Get( message.perception_par_compagnie().elem( i ).oid_compagnie() );
            // $$$$ AGE 2006-02-22: 
            perceptions_.push_back( Perception( agent, (E_PerceptionResult)( 3 - message.perception_par_compagnie().elem( i ).identification_level() ) ) );
        }
        controller_.Update( *this );
    }
}
