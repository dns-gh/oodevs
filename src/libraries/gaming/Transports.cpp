// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Transports.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Transports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Transports::Transports( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, PropertiesDictionary& dico )
    : controller_ ( controller )
    , resolver_   ( resolver )
    , transporter_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: Transports destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Transports::~Transports()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transports::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Transports::CreateDictionary( PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Transports", "Transports/Tow trucks" ), transporter_ );
    dico.Register( *this, tools::translate( "Transports", "Transports/Towed units" ), transported_ );
}

// -----------------------------------------------------------------------------
// Name: Transports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Transports::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_transported_units()  )
    {
        transported_.clear();
        transported_.resize( message.transported_units().elem_size() );
        for( int i = 0; i < message.transported_units().elem_size(); ++i )
            transported_.push_back( resolver_.Find( message.transported_units().elem( i ).id() ) );
    }
    if( message.has_transporting_unit()  )
        transporter_ = resolver_.Find( message.transporting_unit().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Transports::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Transports::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Transports", "Towed units" ) ).Display( "Towed units", transported_ ); // $$$$ AGE 2006-02-22: remove a level
}
