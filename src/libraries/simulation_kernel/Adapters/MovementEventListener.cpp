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
#include "simulation_terrain/TER_World.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "movement";
}

// -----------------------------------------------------------------------------
// Name: MovementEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementEventListener::MovementEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: MovementEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementEventListener::~MovementEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: MovementEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void MovementEventListener::Notify( const core::Model& event )
{
    const unsigned int identifier = event[ "entity" ];
    const core::Model& entity = model_[ "entities" ][ identifier ];
    RoleAction_Moving& role = entity[ "roles/RoleAction_Moving" ].GetUserData< RoleAction_Moving >();
    const core::Model& movement = entity[ "movement" ];
    MT_Vector2D position( movement[ "position/x" ], movement[ "position/y" ] );
    TER_World::GetWorld().ClipPointInsideWorld( position ); // $$$$ MCO : used to be in RolePion_Location::Move but should be done by movement_module I suppose
    const MT_Vector2D direction( movement[ "direction/x" ], movement[ "direction/y" ] );
    const double speed = movement[ "speed" ];
    const double distance = event[ "distance" ];
    role.ApplyMove( position, direction, speed, distance );
}
