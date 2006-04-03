// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ter_pch.h"

#include "TER_PopulationFlowManager.h"
#include "TER_PopulationFlow_ABC.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlowManager::TER_PopulationFlowManager( const MT_Rect& extent )
    : flows_( T_PopulationFlows::T_Rectangle( extent.GetLeft(), extent.GetBottom(), extent.GetRight(), extent.GetTop() ) )
{
    flows_.SetRefinementPolicy( 20 ); // $$$$ AGE 2005-02-02: profile something ?
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlowManager::~TER_PopulationFlowManager()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::UpdatePopulationFlowPosition
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::T_Hint TER_PopulationFlowManager::UpdatePosition( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint )
{
    flows_.ForceErase( &flow, hint );
    return flows_.Insert( &flow );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::RemovePopulationFlow
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
bool TER_PopulationFlowManager::Remove( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint )
{
    return flows_.Erase( &flow, hint );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::GetListWithinCircle
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_PopulationFlowVector& flows ) const
{
    flows.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_PopulationFlows::View view = flows_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationFlow_ABC* pFlow = view.NextElement();
        if( pFlow && pFlow->Intersect2DWithCircle( vCenter, rRadius ) )
            flows.push_back( pFlow );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::GetListIntersectingLine
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::GetListIntersectingLine( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_PopulationFlowVector& flows ) const
{
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vStart.rX_, vStart.rY_ ),
                                                          geometry::Point2< MT_Float >( vEnd.rX_  , vEnd.rY_   ) );
    T_PopulationFlows::View view = flows_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationFlow_ABC* pFlow = view.NextElement();
        if( pFlow && pFlow->Intersect2DWithLine( vStart, vEnd ) )
            flows.push_back( pFlow );
    }
}
