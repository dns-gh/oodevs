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

#include "ter_pch.h"

#include "TER_Agent_ABC.h"

#include "TER_World.h"
#include "TER_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Agent_ABC::TER_Agent_ABC()
    : hint_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_Agent_ABC destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Agent_ABC::~TER_Agent_ABC()
{
    RemoveFromPatch(); // $$$$ SBO 2006-03-31: Pure virtual function call GetLocation()
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
    TER_World::GetWorld().GetAgentManager().Remove( *this, hint_ );
}
