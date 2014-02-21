// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyStates.h"
#include "clients_kernel/Availability.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( kernel::Entity_ABC& entity, Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< DotationType >& dotationResolver, gui::PropertiesDictionary& dico )
    : entity_          ( entity )
    , controller_      ( controller )
    , resolver_        ( resolver )
    , dotationResolver_( dotationResolver )
    , property_        ( tools::translate( "SupplyStates", "Supply system/System enabled" ) )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::~SupplyStates()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void SupplyStates::CreateDictionary( gui::PropertiesDictionary& dico ) const
{
    dico.Register( entity_, property_, bChainEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void SupplyStates::DoUpdate( const sword::LogSupplyState& message )
{
    if( message.has_chain() )
        bChainEnabled_ = message.chain() != 0;
    if( message.has_transporters() )
    {
        dispoTransporters_.resize( message.transporters().elem_size() );
        for( int i = 0; i < message.transporters().elem_size(); ++i )
            dispoTransporters_[i] = Availability( entity_, resolver_, message.transporters().elem( i ) );
    }
    if( message.has_stocks() )
    {
        int nSize = message.stocks().elem_size();
        while( nSize > 0 )
        {
            const sword::DotationStock& value = message.stocks().elem( --nSize );
            Dotation* dotation = Find( value.resource().id() );
            if( dotation )
                dotation->quantity_ = value.quantity();
            else
                Register( value.resource().id(), *new Dotation( dotationResolver_.Get( value.resource().id() ), value.quantity() ) );
        }
    }
    if( message.has_network_stock() )
    {
        int id = message.network_stock().resource().id();
        if( id == 0 )
            resourceNetworkStock_.reset();
        else
            resourceNetworkStock_.reset( new Dotation( dotationResolver_.Get( id ), message.network_stock().quantity() ) );
    }

    if( message.has_chain() )
        controller_.Update( gui::DictionaryUpdated( entity_, property_ ) );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::GetConnectedNetworkStock
// Created: JSR 2012-04-24
// -----------------------------------------------------------------------------
Dotation* SupplyStates::GetConnectedNetworkStock() const
{
    return resourceNetworkStock_.get();
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SupplyStates::Display( Displayer_ABC& displayer ) const
{
    if( bChainEnabled_ )
        displayer.Group( "" ).Display( tools::translate( "SupplyStates", "System status" ),  tools::translate( "SupplyStates", "Enabled" ) );
    else
        displayer.Group( "" ).Display( tools::translate( "SupplyStates", "System status" ),  tools::translate( "SupplyStates", "Disabled" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::IsChainEnabled
// Created: MMC 2013-01-10
// -----------------------------------------------------------------------------
bool SupplyStates::IsChainEnabled() const
{
    return bChainEnabled_;
}
