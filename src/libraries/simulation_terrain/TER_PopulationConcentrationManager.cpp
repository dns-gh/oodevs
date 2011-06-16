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
#include "TER_PopulationConcentrationVisitor_ABC.h"

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::GetListWithinCircle
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_PopulationConcentrationVector& concentrations ) const
{
    concentrations.reserve( 10 );
    pathfind::PointIntersecter< double > intersecter( geometry::Point2< double >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_PopulationConcentrations::View view = concentrations_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationConcentration_ABC* pConcentration = view.NextElement();
        if( pConcentration && pConcentration->Intersect2DWithCircle( vCenter, rRadius ) )
            concentrations.push_back( pConcentration );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::GetListWithinLocalisation
// Created: MGD 2010-05-12
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_PopulationConcentrationVector& concentrations ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    pathfind::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
        , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    T_PopulationConcentrations::View view = concentrations_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationConcentration_ABC* pConcentration = view.NextElement();
        if( pConcentration && pConcentration->IsIntersecting( localisation ) )
            concentrations.push_back( pConcentration );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::GetListIntersectingLine
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationConcentrationVector& concentrations ) const
{
    pathfind::SegmentIntersecter< double > intersecter( geometry::Point2< double >( vStart.rX_, vStart.rY_ ),
                                                          geometry::Point2< double >( vEnd.rX_  , vEnd.rY_   ) );
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

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationManager::Accept
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void TER_PopulationConcentrationManager::Accept( TER_PopulationConcentrationVisitor_ABC& visitor ) const
{
    T_PopulationConcentrations::View view = concentrations_.CreateView();
    while( view.HasMoreElements() )
    {
        TER_PopulationConcentration_ABC* pObject = view.NextElement();
        if( pObject )
            visitor.Visit( *pObject );
    }
}
