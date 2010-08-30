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
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Reinforcements constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reinforcements::Reinforcements( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , reinforced_( 0 )
{
    CreateDictionary( dico );
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
// Name: Reinforcements::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Reinforcements::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Reinforcements", "Reinforcements/Reinforcing" ), reinforced_ );
    dico.Register( *this, tools::translate( "Reinforcements", "Reinforcements/Reinforced by" ), reinforcements_ );
}

// -----------------------------------------------------------------------------
// Name: Reinforcements::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reinforcements::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( message.has_reinforcements()  )
    {
        reinforcements_.clear();
        reinforcements_.reserve( message.reinforcements().elem_size() );
        for( int i = 0; i < message.reinforcements().elem_size(); ++i )
        {
            Agent_ABC* agent = resolver_.Find( message.reinforcements().elem( i ).id() );
            reinforcements_.push_back( agent );
        }
    }

    if( message.has_reinforced_unit()  )
        reinforced_ = resolver_.Find( message.reinforced_unit().id() );

    if( message.has_reinforced_unit()  || message.has_reinforcements()  )
        controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reinforcements::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Reinforcements::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Reinforcements", "Reinforcements" ) )
        .Display( tools::translate( "Reinforcements", "Reinforcing:" ), reinforced_ )
        .Display( tools::translate( "Reinforcements", "Reinforced by:" ), reinforcements_ );
}

// $$$$ AGE 2006-04-06: Draw something ?
