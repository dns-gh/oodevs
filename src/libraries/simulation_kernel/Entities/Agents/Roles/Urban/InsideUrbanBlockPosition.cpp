// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InsideUrbanBlockPosition.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"

#include "AlgorithmsFactories.h"
#include "UrbanLocationComputer_ABC.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include <urban/TerrainObject_ABC.h>
#include "UrbanType.h"
#include "UrbanModel.h"
#include <urban/Model.h>
#include <urban/Architecture.h>
#include <urban/MaterialCompositionType.h>
#include <urban/Soil.h>
#include <urban/StaticModel.h>

MT_Random InsideUrbanBlockPosition::randomGenerator_;

namespace bg = boost::geometry;
using namespace urbanLocation;
// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition constructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
InsideUrbanBlockPosition::InsideUrbanBlockPosition( const urban::TerrainObject_ABC* urbanObject )
    : urbanObject_( urbanObject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition destructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
InsideUrbanBlockPosition::~InsideUrbanBlockPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetFirerPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
geometry::Point2f InsideUrbanBlockPosition::GetFirerPosition( MIL_Agent_ABC& target, UrbanLocationComputer_ABC::Results& firerResult ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    std::vector< geometry::Point2f > points = urbanObject_->GetFootprint()->Intersect( geometry::Segment2f( firerResult.position_, targetResult.position_ ) );
    if( points.empty() )
        throw std::exception( "error in urbanBlock intersection for firer" );
    return GetNearestUrbanBlockPoint( targetResult.position_, points ); 
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetTargetPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
geometry::Point2f InsideUrbanBlockPosition::GetTargetPosition(MIL_Agent_ABC& firer, UrbanLocationComputer_ABC::Results& targetResult ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( firer.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    firer.Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    geometry::Line2f lineTmp( firerResult.position_, targetResult.position_ );
    std::vector< geometry::Point2f > points = urbanObject_->GetFootprint()->Intersect( lineTmp );
    if( points.size() < 2 )
        throw std::exception( " error in urbanBlock intersection for target" );
    geometry::Point2f pfirst = GetNearestUrbanBlockPoint( firerResult.position_, points );
    geometry::Point2f pSecond = GetFurthestUrbanBlockPoint( firerResult.position_, points );
    geometry::Vector2f vector( pfirst, pSecond );
    vector = vector * ( 1 - targetResult.urbanDeployment_ );
    geometry::Point2f pM = pfirst + vector;
    vector = geometry::Vector2f( pM, pSecond ) * randomGenerator_.rand_ii();
    return pM + vector;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetNearestUrbanBlockPoint
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f InsideUrbanBlockPosition::GetNearestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const 
{
    geometry::Point2f nearestPosition;
    float distance = std::numeric_limits< MT_Float >::max();
    for( std::vector< geometry::Point2f >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        float distanceTemp = (*it).Distance( pionPosition );
        if( distanceTemp < distance )
        {
            distance = distanceTemp;
            nearestPosition = *it;
        }
    }
    return nearestPosition;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetFurthestUrbanBlockPoint
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f InsideUrbanBlockPosition::GetFurthestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const
{
    geometry::Point2f furthestPosition;
    float distance = 0;
    for( std::vector< geometry::Point2f >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        float distanceTemp = (*it).Distance( pionPosition );
        if( distanceTemp > distance )
        {
            distance = distanceTemp;
            furthestPosition = *it;
        }
    }
    return furthestPosition;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
float InsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const
{
    std::vector< bg::point_xy< double > > ellipseKeyPoints;
    bg::polygon< bg::point_xy< double > > attritionPolygon;
    ellipseKeyPoints.push_back( bg::point_xy< double >( attritionSurface.GetMajorAxisHighPoint().rX_ + attritionSurface.GetMinorAxisHighPoint().rX_ - attritionSurface.GetCenter().rX_, 
        attritionSurface.GetMajorAxisHighPoint().rY_ + attritionSurface.GetMinorAxisHighPoint().rY_ - attritionSurface.GetCenter().rY_ ) );
    ellipseKeyPoints.push_back( bg::point_xy< double >(attritionSurface.GetMajorAxisHighPoint().rX_ - attritionSurface.GetMinorAxisHighPoint().rX_ + attritionSurface.GetCenter().rX_,
        attritionSurface.GetMajorAxisHighPoint().rY_ - attritionSurface.GetMinorAxisHighPoint().rY_ + attritionSurface.GetCenter().rY_ ) ) ;
    ellipseKeyPoints.push_back( bg::point_xy< double >( 3 * attritionSurface.GetCenter().rX_ - attritionSurface.GetMajorAxisHighPoint().rX_ - attritionSurface.GetMinorAxisHighPoint().rX_,
        3 * attritionSurface.GetCenter().rY_ - attritionSurface.GetMajorAxisHighPoint().rY_ - attritionSurface.GetMinorAxisHighPoint().rY_ ) );
    ellipseKeyPoints.push_back( bg::point_xy< double >( attritionSurface.GetCenter().rX_ - attritionSurface.GetMajorAxisHighPoint().rX_ + attritionSurface.GetMinorAxisHighPoint().rX_,
        attritionSurface.GetCenter().rY_ - attritionSurface.GetMajorAxisHighPoint().rY_ + attritionSurface.GetMinorAxisHighPoint().rY_ ) );
    bg::assign( attritionPolygon, ellipseKeyPoints );
    bg::correct( attritionPolygon );

    geometry::Polygon2f::T_Vertices urbanBlockVertices = urbanObject_->GetFootprint()->Vertices();
    bg::polygon< bg::point_xy< double > > blockGeometry;
    std::vector< bg::point_xy< double > > vectorTemp;
    for ( geometry::Polygon2f::CIT_Vertices it = urbanBlockVertices.begin(); it != urbanBlockVertices.end(); ++it )
    {
        bg::point_xy< double > p( it->X(), it->Y() );
        vectorTemp.push_back( p );

    }
    bg::assign( blockGeometry, vectorTemp );
    bg::correct( blockGeometry );

    std::vector< bg::polygon< bg::point_xy< double > > > polygonResult;
    bg::intersection_inserter<boost::geometry::polygon< bg::point_xy< double > > >(attritionPolygon, blockGeometry, std::back_inserter( polygonResult ) );
    double intersectArea = 0;
    for( std::vector< bg::polygon< bg::point_xy< double > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
    {
        intersectArea += area( *it ); 
    }
    return ( intersectArea / ( urbanObject_->GetFootprint()->ComputeArea() ) ) * result.urbanDeployment_;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeUrbanProtection
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
MT_Float InsideUrbanBlockPosition::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    const urban::Architecture* architecture = urbanObject_->RetrievePhysicalFeature< urban::Architecture >();
    const urban::Soil* soil = urbanObject_->RetrievePhysicalFeature< urban::Soil >();
    if( architecture  && soil )
    {
        unsigned materialID = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId();
        return ( 1 - dotationCategory.GetUrbanAttritionModifer( materialID ) ) * ( soil->GetOccupation() );
    }
    return 0.;
}
