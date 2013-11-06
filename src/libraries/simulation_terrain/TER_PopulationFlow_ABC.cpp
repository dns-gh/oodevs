// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PopulationFlow_ABC.h"
#include "TER_World.h"
#include "TER_PopulationManager.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::TER_PopulationFlow_ABC()
    : hint_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::~TER_PopulationFlow_ABC()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::Intersect2DWithCircle
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
bool TER_PopulationFlow_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const
{
    return GetLocation().Intersect2DWithCircle( vCircleCenter, rRadius );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::Intersect2DWithCircle
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool TER_PopulationFlow_ABC::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const
{
    return GetLocation().Intersect2DWithCircle( vCircleCenter, rRadius, shape );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::Intersect2DWithLine
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
bool TER_PopulationFlow_ABC::Intersect2DWithLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd ) const
{
    return GetLocation().Intersect2D( MT_Line( vStart, vEnd ) );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlow_ABC::IsIntersecting
// Created: LGY 2011-04-04
// -----------------------------------------------------------------------------
bool TER_PopulationFlow_ABC::IsIntersecting( const TER_Localisation& localisation, double precision ) const
{
    return GetLocation().IsIntersecting( localisation, precision );
}
