// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "FireManager.h"
#include "DetonationPDU.h"
#include "UdpNetwork.h"
#include "Time_ABC.h"
#include "protocol/protocol.h"

using namespace plugins::dis;
using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FireManager constructor
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
FireManager::FireManager( UdpNetwork& network, const Time_ABC& time, unsigned char exercise )
    : network_ ( network )
    , time_    ( time )
    , exercise_( exercise )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireManager destructor
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
FireManager::~FireManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireManager::Update
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::Update( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_start_unit_fire() )
        ReceiveFire( wrapper.message().start_unit_fire() );
    if( wrapper.message().has_stop_unit_fire() )
        ReceiveFire( wrapper.message().stop_unit_fire() );
    if( wrapper.message().has_start_fire_effect() )
        UpdateFireEffect( wrapper.message().start_fire_effect() );
    if( wrapper.message().has_control_end_tick() )
        UpdateDetonations();
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const sword::StartUnitFire& message )
{
    if( message.target().has_position() )
    {
        activeFires_[ message.fire().id() ] = message.target().position();
        CreateFire( message.target().position() );
    }
}

// -----------------------------------------------------------------------------
// Name: FireManager::UpdateFireEffect
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void FireManager::UpdateFireEffect( const sword::StartFireEffect& message )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetBurst( 1, 1, message.type() == sword::smoke ? BurstDescriptor::smoke : BurstDescriptor::illumination );
    pdu.SetPosition( message.location().coordinates().elem(0).latitude(), message.location().coordinates().elem(0).longitude(), 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const sword::StopUnitFire& message )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetPosition( activeFires_[ message.fire().id() ].latitude(), activeFires_[ message.fire().id() ].longitude(), 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );

    activeFires_.erase( message.fire().id() );
}

// -----------------------------------------------------------------------------
// Name: FireManager::UpdateDetonations
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::UpdateDetonations()
{
    // $$$$ AGE 2008-04-08: ?
}

// -----------------------------------------------------------------------------
// Name: FireManager::CreateFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::CreateFire( const sword::MsgCoordLatLong& /*position*/ )
{
    // $$$$ AGE 2008-05-05:  ?
}
