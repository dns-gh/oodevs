// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CallbackEventListener.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Decision_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: CallbackEventListener constructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
CallbackEventListener::CallbackEventListener( const core::Model& model, core::Facade& facade, const std::string& event )
    : model_ ( model )
    , facade_( facade )
    , event_ ( event )
{
    facade.Register( event_, *this );
}

// -----------------------------------------------------------------------------
// Name: CallbackEventListener destructor
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
CallbackEventListener::~CallbackEventListener()
{
    facade_.Unregister( event_, *this );
}

// -----------------------------------------------------------------------------
// Name: CallbackEventListener::Notify
// Created: MCO 2012-06-15
// -----------------------------------------------------------------------------
void CallbackEventListener::Notify( const core::Model& callback )
{
    const unsigned int entity = callback[ "entity" ];
    DEC_Decision_ABC& role = model_[ "entities" ][ entity ][ "pion" ].GetUserData< MIL_AgentPion >().GetRole< DEC_Decision_ABC >();
    role.Callback< unsigned int >( callback[ "id" ], callback[ "code" ] );
}
