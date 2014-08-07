// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PopulationFlowManager.h"
#include "TER_PopulationFlow_ABC.h"
#include "TER_PopulationFlowVisitor_ABC.h"
#include "TER_Localisation.h"

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
    // NOTHING
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
// Created: LDC 2012-01-02
// $$$ LDC FIXME Could be templated because it's a copy paste of the next method...
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_PopulationFlowVector& flows ) const
{
    flows.reserve( 10 );
    spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2< double >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_PopulationFlows::View view = flows_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationFlow_ABC* pFlow = view.NextElement();
        if( pFlow && pFlow->Intersect2DWithCircle( vCenter, rRadius ) )
            flows.push_back( pFlow );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::GetListWithinCircle
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_ConstPopulationFlowVector& flows ) const
{
    flows.reserve( 10 );
    spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2< double >( vCenter.rX_, vCenter.rY_ ), rRadius );
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
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2< double >( vStart.rX_, vStart.rY_ ),
                                                                geometry::Point2< double >( vEnd.rX_  , vEnd.rY_   ) );
    T_PopulationFlows::View view = flows_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationFlow_ABC* pFlow = view.NextElement();
        if( pFlow && pFlow->Intersect2DWithLine( vStart, vEnd ) )
            flows.push_back( pFlow );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::GetListWithinLocalisation
// Created: MGD 2010-05-12
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_PopulationFlowVector& flows, double precision ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() ),
                                                                geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    T_PopulationFlows::View view = flows_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_PopulationFlow_ABC* pFlow = view.NextElement();
        if( pFlow && pFlow->IsValid() && pFlow->IsIntersecting( localisation, precision ) )
            flows.push_back( pFlow );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::Accept
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void TER_PopulationFlowManager::Accept( TER_PopulationFlowVisitor_ABC& visitor ) const
{
    flows_.Accept( visitor );
}
