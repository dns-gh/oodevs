// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"

#include "TER_PopulationConcentrationManager.h"
#include "TER_PopulationConcentration_ABC.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentrationManager::TER_PopulationConcentrationManager( const MT_Rect& extent )
    : concentrations_( T_PopulationConcentrations::T_Rectangle( extent.GetLeft(), extent.GetBottom(), extent.GetRight(), extent.GetTop() ) )
{
    concentrations_.SetRefinementPolicy( 20 ); // $$$$ AGE 2005-02-02: profile something ?
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentrationManager::~TER_PopulationConcentrationManager()
{
    //NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::GetListWithinCircle
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_PopulationConcentrationVector& concentrations ) const
{
    concentrations.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_PopulationConcentrations::View view = concentrations_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationConcentration_ABC* pConcentration = view.NextElement();
        if( pConcentration && pConcentration->Intersect2DWithCircle( vCenter, rRadius ) )
            concentrations.push_back( pConcentration );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::GetListIntersectingLine
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationConcentrationVector& concentrations ) const
{
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vStart.rX_, vStart.rY_ ),
                                                          geometry::Point2< MT_Float >( vEnd.rX_  , vEnd.rY_   ) );
    T_PopulationConcentrations::View view = concentrations_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationConcentration_ABC* pConcentration = view.NextElement();
        if( pConcentration && pConcentration->Intersect2DWithLine( vStart, vEnd ) )
            concentrations.push_back( pConcentration );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::UpdatePopulationConcentrationPosition
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationConcentration_ABC::T_Hint TER_PopulationConcentrationManager::UpdatePosition( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint )
{
    concentrations_.ForceErase( &concentration, hint );
    return concentrations_.Insert( &concentration );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::RemovePopulationConcentration
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
bool TER_PopulationConcentrationManager::Remove( TER_PopulationConcentration_ABC& concentration, const TER_PopulationConcentration_ABC::T_Hint& hint )
{
    return concentrations_.Erase( &concentration, hint );
}
