// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Carriers.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: Carriers constructor
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
Carriers::Carriers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Carriers constructor
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
Carriers::Carriers( xml::xistream& xis )
    : dotationFilter_( xis, "dotations", "resources" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Carriers destructor
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
Carriers::~Carriers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Carriers::IsCreation
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
bool Carriers::IsCreation( const sword::SimToClient& wrapper ) const
{
    if( !wrapper.message().has_log_supply_handling_update() )
        return false;
    const sword::LogSupplyHandlingUpdate& msg = wrapper.message().log_supply_handling_update();
    if( !msg.has_convoyer() || msg.convoyer().id() == 0 )
        return false;
    if( !msg.has_state() || msg.state() != sword::LogSupplyHandlingUpdate::convoy_setup )
        return false;
    if( !msg.has_requests() || msg.requests().requests().size() == 0 )
        return false;
    for( int req = 0; req < msg.requests().requests().size(); ++req )
    {
        const sword::SupplyRecipientResourcesRequest& request = msg.requests().requests( req );
        for( int i = 0; i < request.resources().size(); ++i )
        {
            const int dotation = request.resources( i ).resource().id();
            if( dotationFilter_.IsAllowed( dotation ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Carriers::Extract
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
NumericValue Carriers::Extract( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().log_supply_handling_update().convoyer().id();
}

// -----------------------------------------------------------------------------
// Name: Carriers::IsDestruction
// Created: JSR 2011-10-05
// -----------------------------------------------------------------------------
bool Carriers::IsDestruction( const sword::SimToClient& wrapper ) const
{
    if( !wrapper.message().has_log_supply_handling_update() )
        return false;
    const sword::LogSupplyHandlingUpdate& msg = wrapper.message().log_supply_handling_update();
    if( !msg.has_convoyer() || msg.convoyer().id() == 0 )
        return false;
    if( !msg.has_state() || msg.state() != sword::LogSupplyHandlingUpdate::convoy_finished )
        return false;
    return true;
}
