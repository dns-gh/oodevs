// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_Agent_ABC.cpp $
// $Author: Age $
// $Modtime: 31/01/05 18:33 $
// $Revision: 1 $
// $Workfile: TER_Agent_ABC.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"

#include "TER_Agent_ABC.h"

#include "TER_World.h"
#include "TER_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Agent_ABC::TER_Agent_ABC()
    : hint_         ( 0 )
    , pAgentManager_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Agent_ABC::~TER_Agent_ABC()
{
    RemoveFromPatch(); // $$$$ AGE 2006-03-31: pure virtual call sur GetPosition();
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC::UpdatePatch
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Agent_ABC::UpdatePatch()
{
    hint_ = TER_World::GetWorld().GetAgentManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC::InsertInPatch
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Agent_ABC::InsertInPatch()
{
    hint_ = TER_World::GetWorld().GetAgentManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC::RemoveFromPatch
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_Agent_ABC::RemoveFromPatch()
{
    if( pAgentManager_ )
        pAgentManager_->Remove( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC::SetListener
// Created: LDC 2009-06-09
// -----------------------------------------------------------------------------
void TER_Agent_ABC::SetListener( TER_AgentManager& agentManager )
{
    pAgentManager_ = &agentManager;
}
