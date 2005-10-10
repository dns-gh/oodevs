// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationFlow_ABC.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:25 $
// $Revision: 2 $
// $Workfile: TER_PopulationFlow_ABC.cpp $
//
// *****************************************************************************

#include "ter_pch.h"

#include "TER_PopulationFlow_ABC.h"

#include "TER_World.h"
#include "TER_PopulationManager.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::TER_PopulationFlow_ABC()
    : hint_     ( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::~TER_PopulationFlow_ABC()
{
    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::UpdatePatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationFlow_ABC::UpdatePatch()
{
    hint_ = TER_World::GetWorld().GetPopulationManager().GetFlowManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::InsertInPatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationFlow_ABC::InsertInPatch()
{
    hint_ = TER_World::GetWorld().GetPopulationManager().GetFlowManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::RemoveFromPatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationFlow_ABC::RemoveFromPatch()
{
    TER_World::GetWorld().GetPopulationManager().GetFlowManager().Remove( *this, hint_ );
}
