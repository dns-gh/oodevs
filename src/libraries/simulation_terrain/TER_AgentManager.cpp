// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_AgentManager.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:04 $
// $Revision: 2 $
// $Workfile: TER_AgentManager.cpp $
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_AgentManager.h"
#include "TER_Agent_ABC.h"
#include "TER_Localisation.h"
#include "TER_Polygon.h"
#include "MT_Tools/MT_Ellipse.h"
#include "pathfind/SpatialContainerTraits.h"
#include "geometry/Point2.h"

// -----------------------------------------------------------------------------
// Name: TER_AgentManager constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_AgentManager::TER_AgentManager( const MT_Rect& extent )
    : agents_( T_Agents::T_Rectangle( extent.GetLeft(), extent.GetBottom(), extent.GetRight(), extent.GetTop() ) )
{
    agents_.SetRefinementPolicy( 20 ); // $$$$ AGE 2005-02-02: profile something ?
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_AgentManager::~TER_AgentManager()
{
    //NOTHING
}

namespace
{
    template< typename Where >
    class AgentFinder
    {
    public:
        AgentFinder( const Where& where, TER_AgentManager::T_AgentVector& agents ) : where_( where ), agents_( agents ) {};
        bool operator()( TER_Agent_ABC* pAgent )
        {
            if( pAgent && where_.IsInside( pAgent->GetPosition() ) )
                agents_.push_back( pAgent );
            return true;
        }
    private:
        AgentFinder& operator=( const AgentFinder& );
        const Where& where_;
        TER_AgentManager::T_AgentVector& agents_;
    };

    template< typename Where >
    class AgentFinderPrecision
    {
    public:
        AgentFinderPrecision( const Where& where, MT_Float rPrecision, TER_AgentManager::T_AgentVector& agents ) : where_( where ), rPrecision_( rPrecision ), agents_( agents ) {};
        bool operator()( TER_Agent_ABC* pAgent )
        {
            if( pAgent && where_.IsInside( pAgent->GetPosition(), rPrecision_ ) )
                agents_.push_back( pAgent );
            return true;
        }
    private:
        AgentFinderPrecision& operator=( const AgentFinderPrecision& );
        const Where& where_;
        const MT_Float rPrecision_;
        TER_AgentManager::T_AgentVector& agents_;
    };

    class Circle
    {
    public:
        Circle( const MT_Vector2D& center, MT_Float rRadius ) : center_( center ), rSquareRadius_( rRadius * rRadius ) {};
        bool IsInside( const MT_Vector2D& pos ) const
        {
            return center_.SquareDistance( pos ) < rSquareRadius_;
        };
    private:
        MT_Vector2D center_;
        MT_Float rSquareRadius_;
    };
};

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinEllipse
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agents ) const
{
    const MT_Rect boundingBox = ellipse.GetBoundingBox();
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                        , geometry::Point2<MT_Float>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinder< MT_Ellipse > finder( ellipse, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );

}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_AgentVector& agents ) const
{
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( vCenter.rX_ - rRadius, vCenter.rY_ - rRadius )
                                                        , geometry::Point2<MT_Float>( vCenter.rX_ + rRadius, vCenter.rY_ + rRadius ) );
    AgentFinder< Circle > finder( Circle( vCenter, rRadius ), agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinLocalisation
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agents ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                        , geometry::Point2<MT_Float>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinder< TER_Localisation > finder( localisation, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinLocalisation
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agents, MT_Float rPrecision ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                        , geometry::Point2<MT_Float>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinderPrecision< TER_Localisation > finder( localisation, rPrecision, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinPolygon
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinPolygon( const TER_Polygon& polygon, T_AgentVector& agents ) const
{
    const MT_Rect boundingBox = polygon.GetBoundingBox();
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                        , geometry::Point2<MT_Float>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinder< TER_Polygon > finder( polygon, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::UpdatePosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Agent_ABC::T_Hint TER_AgentManager::UpdatePosition( TER_Agent_ABC& agent, const TER_Agent_ABC::T_Hint& hint )
{
    agents_.ForceErase( &agent, hint );
    return agents_.Insert( &agent );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::Remove
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_AgentManager::Remove( TER_Agent_ABC& agent, const TER_Agent_ABC::T_Hint& hint )
{
    return agents_.Erase( &agent, hint );
}
