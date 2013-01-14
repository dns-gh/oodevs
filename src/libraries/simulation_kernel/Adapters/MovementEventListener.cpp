// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MovementEventListener.h"
#include "RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "simulation_terrain/TER_World.h"
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: MovementEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementEventListener::MovementEventListener( const core::Model& model, core::Facade& facade )
    : EventListenerBase( facade, "movement" )
    , model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MovementEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementEventListener::~MovementEventListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MovementEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void MovementEventListener::Notify( const core::Model& event )
{
    const unsigned int identifier = event[ "entity" ];
    const core::Model& entity = model_[ "entities" ][ identifier ];
    RoleAction_Moving& role = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< RoleAction_Moving >();
    const core::Model& movement = entity[ "movement" ];
    MT_Vector2D position( movement[ "position/x" ], movement[ "position/y" ] );
    TER_World::GetWorld().ClipPointInsideWorld( position ); // $$$$ MCO : used to be in RolePion_Location::Move but should be done by movement_module I suppose
    const MT_Vector2D direction( movement[ "direction/x" ], movement[ "direction/y" ] );
    const double speed = movement[ "speed" ];
    const double distance = event[ "distance" ];
    role.ApplyMove( position, direction, speed, distance );
}
