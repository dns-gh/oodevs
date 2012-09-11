// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MovementCallbackEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Decision_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "movement callback";
}

// -----------------------------------------------------------------------------
// Name: MovementCallbackEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementCallbackEventListener::MovementCallbackEventListener( const core::Model& model, core::Facade& facade )
    : model_ ( model )
    , facade_( facade )
{
    facade.Register( event, *this );
}

// -----------------------------------------------------------------------------
// Name: MovementCallbackEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
MovementCallbackEventListener::~MovementCallbackEventListener()
{
    facade_.Unregister( event, *this );
}

// -----------------------------------------------------------------------------
// Name: MovementCallbackEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void MovementCallbackEventListener::Notify( const core::Model& callback )
{
    const unsigned int entity = callback[ "entity" ];
    DEC_Decision_ABC& role = model_[ "entities" ][ entity ][ "pion" ].GetUserData< MIL_AgentPion >().GetRole< DEC_Decision_ABC >();
    role.Callback< unsigned int >( callback[ "id" ], callback[ "code" ] );
}
