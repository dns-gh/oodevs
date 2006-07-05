// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PerceptionMap.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: PerceptionMap constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
PerceptionMap::PerceptionMap( Controller& controller, const Resolver_ABC< Agent >& resolver )
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
void PerceptionMap::DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    if( message.m.perception_par_compagniePresent )
    {
        perceptions_.clear();
        perceptions_.reserve( message.perception_par_compagnie.n );
        for( uint i = 0; i < message.perception_par_compagnie.n; ++i )
        {
            const Agent& agent = resolver_.Get( message.perception_par_compagnie.elem[i].oid_compagnie );
            // $$$$ AGE 2006-02-22: 
            perceptions_.push_back( Perception( agent, (E_PerceptionResult)( 3 - message.perception_par_compagnie.elem[i].identification_level ) ) );
        }
        controller_.Update( *this );
    }
}
