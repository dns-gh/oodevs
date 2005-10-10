// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationConcentration_ABC.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:25 $
// $Revision: 2 $
// $Workfile: TER_PopulationConcentration_ABC.cpp $
//
// *****************************************************************************

#include "ter_pch.h"

#include "TER_PopulationConcentration_ABC.h"

#include "TER_World.h"
#include "TER_PopulationManager.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentration_ABC::TER_PopulationConcentration_ABC()
    : hint_     ( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentration_ABC::~TER_PopulationConcentration_ABC()
{
    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::UpdatePatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationConcentration_ABC::UpdatePatch()
{
    hint_ = TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::InsertInPatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationConcentration_ABC::InsertInPatch()
{
    hint_ = TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().UpdatePosition( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::RemoveFromPatch
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationConcentration_ABC::RemoveFromPatch()
{
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().Remove( *this, hint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::Intersect2DWithCircle
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
bool TER_PopulationConcentration_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const
{
    return GetLocation().Intersect2DWithCircle( vCircleCenter, rRadius );
}
