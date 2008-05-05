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
#include "UdpNetwork.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: FireManager constructor
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
FireManager::FireManager( UdpNetwork& network )
    : network_( network )
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
        ReceiveFire( *message.msg.u.msg_start_unit_fire );
    case T_MsgsSimToClient_msg_msg_stop_unit_fire:
        ReceiveFire( *message.msg.u.msg_stop_unit_fire );
    case T_MsgsSimToClient_msg_msg_control_end_tick:
        UpdateDetonations();
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
// Name: FireManager::ReceiveFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::ReceiveFire( const ASN1T_MsgStopUnitFire& fire )
{
    activeFires_.erase( fire.fire_oid );
}

// -----------------------------------------------------------------------------
// Name: FireManager::UpdateDetonations
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::UpdateDetonations()
{
    // $$$$ AGE 2008-04-08: 
}

// -----------------------------------------------------------------------------
// Name: FireManager::CreateFire
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
void FireManager::CreateFire( const ASN1T_CoordLatLong& position )
{

}
