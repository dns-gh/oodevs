// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PopulationConcentration_ABC.h"
#include "TER_Localisation.h"
#include "TER_World.h"
#include "TER_PopulationManager.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentration_ABC::TER_PopulationConcentration_ABC()
    : hint_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentration_ABC::~TER_PopulationConcentration_ABC()
{
    // NOTHING
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
bool TER_PopulationConcentration_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    return GetLocation().Intersect2DWithCircle( vCircleCenter, rRadius );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::Intersect2DWithLine
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
bool TER_PopulationConcentration_ABC::Intersect2DWithLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd ) const
{
    return GetLocation().Intersect2D( MT_Line( vStart, vEnd ) );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentration_ABC::IsIntersecting
// Created: LGY 2011-04-04
// -----------------------------------------------------------------------------
bool TER_PopulationConcentration_ABC::IsIntersecting( const TER_Localisation& localisation, double precision ) const
{
    return GetLocation().IsIntersecting( localisation, precision );
}
