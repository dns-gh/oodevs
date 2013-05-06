// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Analyzer.h"
#include "TER_Localisation.h"
#include "TER_NodeFunctor_ABC.h"
#include "TER_StaticData.h"
#include "TER_Polygon.h"
#include <spatialcontainer/Node.h>
#include <analysis/TerrainAnalyzer.h>

// -----------------------------------------------------------------------------
// Name: TER_Analyzer constructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_Analyzer::TER_Analyzer( const TER_StaticData& staticData )
{
    pAnalyzer_ = new TerrainAnalyzer( staticData );
    pAnalyzer_->SetPickingDistances( 1000.f, 10000.f ); // minpicking, maxpicking
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer destructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_Analyzer::~TER_Analyzer()
{
    delete pAnalyzer_;
}

namespace
{
    class NodeCircleFinder
    {
    public:
        NodeCircleFinder( TER_NodeFunctor_ABC& functor, const MT_Vector2D& vCenter, double rRadius )
            : functor_      ( functor )
            , center_       ( vCenter )
            , rSquareRadius_( rRadius * rRadius )
        {
            // NOTHING
        }
        bool operator()( const spatialcontainer::Node& node )
        {
            const MT_Vector2D nodePos( node.X(), node.Y() );
            if( center_.SquareDistance( nodePos ) < rSquareRadius_ )
                functor_.Visit( nodePos, TerrainData::BuildData( node ) );
            return true;
        }
    private:
        NodeCircleFinder& operator=( const NodeCircleFinder& );
        TER_NodeFunctor_ABC& functor_;
        MT_Vector2D center_;
        double rSquareRadius_;
    };

    inline geometry::Point2f MakePoint( const MT_Vector2D& v )
    {
        return geometry::Point2f( static_cast< float >( v.rX_ ), static_cast< float >( v.rY_ ) );
    };
    inline boost::shared_ptr< MT_Vector2D > MakeVectorPointer( const geometry::Point2f& p )
    {
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( static_cast< double >( p.X() ), static_cast< double >( p.Y() ) ) );
    };
    inline geometry::Polygon2f MakePolygon( const TER_Polygon& p )
    {
        geometry::Polygon2f polygon;
        const T_PointVector& points = p.GetBorderPoints();
        for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
            polygon.Add( MakePoint( *it ) );
        return polygon;
    };
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::ApplyOnNodesWithinCircle
// Created: AGE 2005-02-23
// -----------------------------------------------------------------------------
void TER_Analyzer::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const
{
    NodeCircleFinder finder( bestNodeFunction, vCenter, rRadius );
    pAnalyzer_->ApplyOnNodesInCircle( MakePoint( vCenter ), static_cast< float >( rRadius ), finder );
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindCrossroadsWithinCircle
// Created: RPD 2009-08-18
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_Analyzer::FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    std::vector< spatialcontainer::Node* > nodes = pAnalyzer_->FindCrossroadNodesWithinCircle( MakePoint( center ), radius );
    for( std::vector< spatialcontainer::Node* >::const_iterator it = nodes.begin(); it != nodes.end(); ++it )
        result.push_back( MakeVectorPointer( **it ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindSafetyPositionsWithinCircle
// Created: LDC 2010-10-28
// -----------------------------------------------------------------------------
void TER_Analyzer::FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance, std::vector< boost::shared_ptr< MT_Vector2D > >& positions )
{
    std::vector< geometry::Point2f > safetyPoints;
    pAnalyzer_->FindSafetyPositionsWithinCircle( MakePoint( center ), radius, safetyDistance, safetyPoints );
    for( std::vector< geometry::Point2f >::const_iterator it = safetyPoints.begin(); it != safetyPoints.end(); ++it )
        positions.push_back( MakeVectorPointer( *it ) );
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindRoadsOnBorderOfPolygon
// Created: CMA 2011-09-01
// -----------------------------------------------------------------------------
void TER_Analyzer::FindRoadsOnBorderOfPolygon( const TER_Polygon& polygon, std::vector< boost::shared_ptr< MT_Vector2D > >& positions )
{
    std::vector< geometry::Point2f > roadPoints;
    pAnalyzer_->FindRoadsOnBorderOfPolygon( MakePolygon( polygon ), roadPoints );
    for( std::vector< geometry::Point2f >::const_iterator it = roadPoints.begin(); it != roadPoints.end(); ++it )
        positions.push_back( MakeVectorPointer( *it ) );
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindAllPositions
// Created: BCI 2011-04-01
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_Analyzer::FindAllPositions( const MT_Vector2D& center, float radius )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    std::vector< spatialcontainer::Node* > nodes = pAnalyzer_->FindNodesWithinCircle( MakePoint( center ), radius );
    for( std::vector< spatialcontainer::Node* >::const_iterator it = nodes.begin(); it != nodes.end(); ++it )
        result.push_back( MakeVectorPointer( **it ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindTerrainDataWithinCircle
// Created: BCI 2010-12-20
// -----------------------------------------------------------------------------
TerrainData TER_Analyzer::FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius )
{
    TerrainData result;
    std::vector< spatialcontainer::Node* > nodes = pAnalyzer_->FindNodesWithinCircle( MakePoint( center ), radius );
    for( std::vector< spatialcontainer::Node* >::const_iterator iterator = nodes.begin(); iterator != nodes.end(); ++iterator )
        result.Merge( TerrainData::BuildData( **iterator ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::FindTerrainDataWithinPolygon
// Created: JSR 2012-02-03
// -----------------------------------------------------------------------------
TerrainData TER_Analyzer::FindTerrainDataWithinPolygon( const TER_Polygon& polygon )
{
    TerrainData result;
    std::vector< spatialcontainer::Node* > nodes = pAnalyzer_->FindNodesWithinPolygon( MakePolygon( polygon ) );
    for( std::vector< spatialcontainer::Node* >::const_iterator iterator = nodes.begin(); iterator != nodes.end(); ++iterator )
        result.Merge( TerrainData::BuildData( **iterator ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::Pick
// Created: BCI 2011-03-04
// -----------------------------------------------------------------------------
TerrainData TER_Analyzer::Pick( const MT_Vector2D& pos )
{
    return pAnalyzer_->Pick( MakePoint( pos ), 100000.f );
}

// -----------------------------------------------------------------------------
// Name: TER_Analyzer::GetTerrainData
// Created: LDC 2013-05-06
// -----------------------------------------------------------------------------
TerrainData TER_Analyzer::GetTerrainData( const TER_Localisation& localisation )
{
    TerrainData data;
    switch( localisation.GetType() )
    {
    case TER_Localisation::eCircle :
        data = FindTerrainDataWithinCircle( localisation.GetCircleCenter(), static_cast< float >( localisation.GetCircleRadius() ) );
        break;
    case TER_Localisation::eLine :
        {
            TER_Localisation tmp = localisation;
            tmp.Scale( 100. );
            TER_Polygon polygon;
            polygon.Reset( tmp.GetPoints() );
            data = FindTerrainDataWithinPolygon( polygon );
        }
        break;
    case TER_Localisation::ePolygon :
        {
            TER_Polygon polygon;
            polygon.Reset( localisation.GetPoints() );
            data = FindTerrainDataWithinPolygon( polygon );
        }
        break;
    case TER_Localisation::ePoint :
        data = Pick( localisation.ComputeBarycenter() );
        break;
    default:
        assert( false );
        break;
    }
    return data;
}