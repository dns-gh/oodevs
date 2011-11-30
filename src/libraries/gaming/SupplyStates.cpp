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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< DotationType >& dotationResolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , dotationResolver_( dotationResolver )
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
void SupplyStates::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "SupplyStates", "Supply system/System enabled" ), bChainEnabled_ );
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
            dispoTransporters_[i] = SupplyAvailability( resolver_, message.transporters().elem( i ) );
    }
    if( message.has_stocks() )
    {
        int nSize = message.stocks().elem_size();
        while( nSize > 0 )
        {
            const sword::DotationStock& value = message.stocks().elem( --nSize );
            DotationType& type = dotationResolver_.Get( value.resource().id() );
            Dotation* dotation = Find( value.resource().id() );
            if( dotation )
                dotation->quantity_ = value.quantity();
            else
                Register( value.resource().id(), *new Dotation( type, value.quantity() ) );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void SupplyStates::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "SupplyStates", "Supply system state" ) )
                .Display( tools::translate( "SupplyStates", "System status" ),
                    bChainEnabled_ ? tools::translate( "SupplyStates", "Enabled" )
                                   : tools::translate( "SupplyStates", "Disabled" ) );
}
