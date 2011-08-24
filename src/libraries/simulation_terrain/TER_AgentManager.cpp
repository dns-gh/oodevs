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

#include "simulation_terrain_pch.h"
#include "TER_AgentManager.h"
#include "TER_Agent_ABC.h"
#include "TER_Localisation.h"
#include "TER_Polygon.h"
#include "TER_AgentVisitor_ABC.h"
#include "MT_Tools/MT_Ellipse.h"

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
    // NOTHING
}

namespace
{
    template< typename Where >
    class AgentFinder
    {
    public:
        AgentFinder( const Where& where, TER_AgentManager::T_AgentVector& agents )
            : where_ ( where )
            , agents_( agents )
        {};
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
        AgentFinderPrecision( const Where& where, double rPrecision, TER_AgentManager::T_AgentVector& agents )
            : where_     ( where )
            , rPrecision_( rPrecision )
            , agents_    ( agents )
        {};
        bool operator()( TER_Agent_ABC* pAgent )
        {
            if( pAgent && where_.IsInside( pAgent->GetPosition(), rPrecision_ ) )
                agents_.push_back( pAgent );
            return true;
        }
    private:
        AgentFinderPrecision& operator=( const AgentFinderPrecision& );
        const Where& where_;
        const double rPrecision_;
        TER_AgentManager::T_AgentVector& agents_;
    };

    class Circle
    {
    public:
        Circle( const MT_Vector2D& center, double rRadius )
            : center_       ( center )
            , rSquareRadius_( rRadius * rRadius )
        {}
        bool IsInside( const MT_Vector2D& pos ) const
        {
            return center_.SquareDistance( pos ) < rSquareRadius_;
        };
    private:
        MT_Vector2D center_;
        double rSquareRadius_;
    };
};

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinEllipse
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinEllipse( const MT_Ellipse& ellipse, T_AgentVector& agents ) const
{
    const MT_Rect boundingBox = ellipse.GetBoundingBox();
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                                , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinder< MT_Ellipse > finder( ellipse, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_AgentVector& agents ) const
{
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( vCenter.rX_ - rRadius, vCenter.rY_ - rRadius )
                                                                , geometry::Point2<double>( vCenter.rX_ + rRadius, vCenter.rY_ + rRadius ) );
    Circle circle( vCenter, rRadius );
    AgentFinder< Circle > finder( circle, agents );
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
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                                , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
    AgentFinder< TER_Localisation > finder( localisation, agents );
    agents.reserve( 10 );
    agents_.Apply( intersecter, finder );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::GetListWithinLocalisation
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void TER_AgentManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_AgentVector& agents, double rPrecision ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                                , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
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
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                                , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );
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
    agent.SetListener( *this );
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

// -----------------------------------------------------------------------------
// Name: TER_AgentManager::Accept
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void TER_AgentManager::Accept( TER_AgentVisitor_ABC& visitor ) const
{
    agents_.Accept( visitor );
}
