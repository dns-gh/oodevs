// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Reinforcements.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DataDictionary.h"

// -----------------------------------------------------------------------------
// Name: Reinforcements constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reinforcements::Reinforcements( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, DataDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , reinforced_( 0 )
{
    dico.Register( "Renforts/Renforce", reinforced_ );
    dico.Register( "Renforts/Est renforc� par", reinforcements_ );
}

// -----------------------------------------------------------------------------
// Name: Reinforcements destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reinforcements::~Reinforcements()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Reinforcements::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reinforcements::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.pions_renforcantPresent )
    {
        reinforcements_.clear();
        reinforcements_.reserve( message.pions_renforcant.n );
        for( uint i = 0; i < message.pions_renforcant.n; ++i )
        {
            Agent_ABC* agent = resolver_.Find( message.pions_renforcant.elem[i] );
            reinforcements_.push_back( agent );
        }
    }

    if( message.m.pion_renforcePresent )
        reinforced_ = resolver_.Find( message.pion_renforce );

    if( message.m.pion_renforcePresent || message.m.pions_renforcantPresent )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reinforcements::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Reinforcements::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Renforts" )
        .Display( "Renforce:", reinforced_ )
        .Display( "Est renforc� par:", reinforcements_ );
}

// $$$$ AGE 2006-04-06: Draw something ?
