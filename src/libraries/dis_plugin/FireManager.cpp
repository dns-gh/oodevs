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

using namespace plugins::dis;

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
void FireManager::Update( const ASN1T_MsgsSimToClient& message )
{
    switch( message.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_start_unit_fire:
        ReceiveFire( *message.msg.u.msg_start_unit_fire ); break;
    case T_MsgsSimToClient_msg_msg_stop_unit_fire:
        ReceiveFire( *message.msg.u.msg_stop_unit_fire ); break;
    case T_MsgsSimToClient_msg_msg_start_fire_effect:
        UpdateFireEffect( *message.msg.u.msg_start_fire_effect ); break;
    case T_MsgsSimToClient_msg_msg_control_end_tick:
        UpdateDetonations(); break;
    };
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const ASN1T_MsgStartUnitFire& fire )
{
    if( fire.target.t == T_MsgStartUnitFire_target_position )
    {
        activeFires_[ fire.fire_oid ] = *fire.target.u.position;
        CreateFire( *fire.target.u.position );
    }
}

// -----------------------------------------------------------------------------
// Name: FireManager::UpdateFireEffect
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
void FireManager::UpdateFireEffect( const ASN1T_MsgStartFireEffect& fire )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetBurst( 1, 1, fire.type == EnumFireEffectType::fumigene ? BurstDescriptor::smoke : BurstDescriptor::illumination );
    pdu.SetPosition( fire.location.coordinates.elem->latitude, fire.location.coordinates.elem->longitude, 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );
}

// -----------------------------------------------------------------------------
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const ASN1T_MsgStopUnitFire& fire )
{
    DetonationPDU pdu( EntityIdentifier( 1, 1, 1 ), time_.GetTime(), exercise_ );
    pdu.SetPosition( activeFires_[ fire.fire_oid ].latitude, activeFires_[ fire.fire_oid ].longitude, 0 );  // $$$$ AGE 2008-05-05: altitude
    network_.Send( pdu );

    activeFires_.erase( fire.fire_oid );
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
void FireManager::CreateFire( const ASN1T_CoordLatLong& position )
{
    // $$$$ AGE 2008-05-05:  ?
}

