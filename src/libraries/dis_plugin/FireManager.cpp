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

using namespace Common;

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
void FireManager::Update( const MsgSimToClient& wrapper )
{
    if ( wrapper.message().has_start_unit_fire() )
        ReceiveFire( wrapper.message().start_unit_fire() ); 
    if ( wrapper.message().has_stop_unit_fire() )
        ReceiveFire( wrapper.message().stop_unit_fire() ); 
    if ( wrapper.message().has_start_fire_effect() )
        UpdateFireEffect( wrapper.message().start_fire_effect() ); 
    if ( wrapper.message().has_control_end_tick() )
        UpdateDetonations(); 
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const MsgStartUnitFire& fire )
{
    if( fire.target().has_position() )
    {
        activeFires_[ fire.fire_oid() ] = fire.target().position();
        CreateFire( fire.target().position() );
    }
}

// -----------------------------------------------------------------------------
// Name: FireManager::UpdateFireEffect
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void FireManager::UpdateFireEffect( const MsgStartFireEffect& fire )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetBurst( 1, 1, fire.type() == EnumFireEffectType::fumigene ? BurstDescriptor::smoke : BurstDescriptor::illumination );
    pdu.SetPosition( fire.location().coordinates().elem(0).latitude(), fire.location().coordinates().elem(0).longitude(), 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const MsgStopUnitFire& fire )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetPosition( activeFires_[ fire.fire_oid() ].latitude(), activeFires_[ fire.fire_oid() ].longitude(), 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );

    activeFires_.erase( fire.fire_oid() );
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
void FireManager::CreateFire( const MsgCoordLatLong& /*position*/ )
{
    // $$$$ AGE 2008-05-05:  ?
}
