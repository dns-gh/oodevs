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
#include "clients_gui/DictionaryUpdated.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"
#include <boost/range/algorithm_ext/erase.hpp>  

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Transports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Transports::Transports( kernel::Entity_ABC& entity, Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, gui::PropertiesDictionary& dico )
    : entity_( entity )
    , controller_( controller )
    , resolver_( resolver )
    , transporter_( 0 )
    , transportersReady_( false )
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
void Transports::CreateDictionary( gui::PropertiesDictionary& dico ) const
{
    dico.Register( entity_, tools::translate( "Transports", "Transports/Tow trucks" ), transporter_ );
    dico.Register( entity_, tools::translate( "Transports", "Transports/Towed units" ), transported_ );
}

// -----------------------------------------------------------------------------
// Name: Transports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Transports::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_transported_units() )
    {
        transported_.clear();
        transported_.resize( message.transported_units().elem_size() );
        for( int i = 0; i < message.transported_units().elem_size(); ++i )
            transported_[ i ] = resolver_.Find( message.transported_units().elem( i ).id() );
    }
    if( message.has_transporting_unit() )
        transporter_ = resolver_.Find( message.transporting_unit().id() );
    if( message.has_transported_units() || message.has_transporting_unit() )
        controller_.Update( gui::DictionaryUpdated( entity_, tools::translate( "Transports", "Transports" ) ) );
    if( message.has_transporters_available() )
        transportersReady_ = message.transporters_available() != 0;
}

// -----------------------------------------------------------------------------
// Name: Transports::DoUpdate
// Created: LDC 2013-09-13
// -----------------------------------------------------------------------------
void Transports::DoUpdate( const sword::UnitDestruction& destruction )
{
    const unsigned int id = destruction.unit().id();
    if( transporter_ && transporter_->GetId() == id )
        transporter_ = 0;
    boost::remove_erase_if( transported_,
        [&]( const kernel::Agent_ABC* agent ) { return id == agent->GetId(); } );
}

// -----------------------------------------------------------------------------
// Name: Transports::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void Transports::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Transports", "Towed units" ) ).Display( "Towed units", transported_ ); // $$$$ AGE 2006-02-22: remove a level
}

// -----------------------------------------------------------------------------
// Name: Transports::CanRetrieveTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
bool Transports::CanRetrieveTransporters() const
{
    return !transportersReady_;
}
