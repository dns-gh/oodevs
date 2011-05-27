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
#include "Entities/Objects/MaterialAttribute.h"
#include "AlgorithmsFactories.h"
#include "UrbanLocationComputer_ABC.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include "PHY_MaterialCompositionType.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Tools/MIL_Geometry.h"
#include "MT_Tools/MT_Ellipse.h"
#include <urban/GeometryAttribute.h>
#include <urban/TerrainObject_ABC.h>
#pragma warning( push, 0 )
#include <boost/geometry/geometry.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;
using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition constructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
InsideUrbanBlockPosition::InsideUrbanBlockPosition( const UrbanObjectWrapper& urbanObject )
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
MT_Vector2D InsideUrbanBlockPosition::GetFirerPosition( MIL_Agent_ABC& target, UrbanLocationComputer_ABC::Results& firerResult ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();
    TER_DistanceLess cmp ( targetResult.position_ );
    T_PointSet collisions( cmp );
    if( !urbanObject_.GetLocalisation().Intersect2D( MT_Line( targetResult.position_, firerResult.position_ ), collisions, 0 ) )
        return firerResult.position_; //// $$$$ _RC_ SBO 2010-07-07: devrait etre throw std::exception( "error in urbanBlock intersection for firer" );
    return *collisions.begin(); // Nearest point from targetResult
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetTargetPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
MT_Vector2D InsideUrbanBlockPosition::GetTargetPosition(MIL_Agent_ABC& firer, UrbanLocationComputer_ABC::Results& targetResult ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( firer.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    firer.Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();
    TER_DistanceLess cmp ( firerResult.position_ );
    T_PointSet collisions( cmp );
    urbanObject_.GetLocalisation().Intersect2D( MT_Line( targetResult.position_, firerResult.position_ ), collisions, 0 );
    if( collisions.size() < 2 )
        return targetResult.position_;  // $$$$ _RC_ SBO 2010-07-07: devrait etre throw std::exception( " error in urbanBlock intersection for target" );
    MT_Vector2D vector( *collisions.rbegin() - *collisions.begin() );
    vector *= ( 1 - targetResult.urbanDeployment_ );
    MT_Vector2D pM = *collisions.begin() + vector;
    return pM + MIL_Random::rand_ii() * ( *collisions.rbegin() - pM );
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
float InsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const
{
    std::vector< bg::point_xy< double > > ellipseKeyPoints;
    bg::polygon< bg::point_xy< double > > attritionPolygon;
    MT_Vector2D major = attritionSurface.GetMajorAxisHighPoint();
    MT_Vector2D minor = attritionSurface.GetMinorAxisHighPoint();
    MT_Vector2D center = attritionSurface.GetCenter();
    ellipseKeyPoints.push_back( bg::point_xy< double >( major.rX_ + minor.rX_ - center.rX_, major.rY_ + minor.rY_ - center.rY_ ) );
    ellipseKeyPoints.push_back( bg::point_xy< double >( major.rX_ - minor.rX_ + center.rX_, major.rY_ - minor.rY_ + center.rY_ ) ) ;
    ellipseKeyPoints.push_back( bg::point_xy< double >( 3 * center.rX_ - major.rX_ - minor.rX_, 3 * center.rY_ - major.rY_ - minor.rY_ ) );
    ellipseKeyPoints.push_back( bg::point_xy< double >( center.rX_ - major.rX_ + minor.rX_, center.rY_ - major.rY_ + minor.rY_ ) );
    bg::assign( attritionPolygon, ellipseKeyPoints );
    bg::correct( attritionPolygon );

    const T_PointVector& urbanBlockVertices = urbanObject_.GetLocalisation().GetPoints();
    bg::polygon< bg::point_xy< double > > blockGeometry;
    std::vector< bg::point_xy< double > > vectorTemp;
    for( CIT_PointVector it = urbanBlockVertices.begin(); it != urbanBlockVertices.end(); ++it )
    {
        bg::point_xy< double > p( it->rX_, it->rY_ );
        vectorTemp.push_back( p );
    }
    bg::assign( blockGeometry, vectorTemp );
    bg::correct( blockGeometry );

    std::vector< bg::polygon< bg::point_xy< double > > > polygonResult;
    bg::intersection_inserter< bg::polygon< bg::point_xy< double > > >(attritionPolygon, blockGeometry, std::back_inserter( polygonResult ) );
    double intersectArea = 0;
    for( std::vector< bg::polygon< bg::point_xy< double > > >::const_iterator it = polygonResult.begin(); it != polygonResult.end(); ++it  )
        intersectArea += area( *it );
    return static_cast< float >( ( intersectArea / ( urbanObject_.GetLocalisation().GetArea() ) ) * result.urbanDeployment_ );
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
float InsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const geometry::Polygon2f& polygon, float modicator ) const
{
    // $$$$ _RC_ JSR 2011-05-25: TODO cleaner cette méthode (warnings) et passer un TER_Polygon (ou un truc comme ça) à la place du polygon2f, pour ne pas utiliser le IntersectionArea avec les TER_Location
    float urbanObjectArea = urbanObject_.GetLocalisation().GetArea();
    if( modicator > result.urbanDeployment_ ) // SLG : permet d'éviter des incohérence dans la percpetion d'unité quand la cible passe en état posté.
    {
        if( polygon.IsInside( geometry::Point2f( result.position_.rX_, result.position_.rY_ ) ) )
            return 1.0;
        else
            return 0.0;
    }
    else if( urbanObjectArea )
    {
        float intersectArea = MIL_Geometry::IntersectionArea( polygon, urbanObject_.GetObject().Get< urban::GeometryAttribute >().Geometry() );
        return ( intersectArea / urbanObjectArea ) * result.urbanDeployment_;
    }
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeUrbanProtection
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
double InsideUrbanBlockPosition::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    if( const MaterialAttribute* materialAttribute = urbanObject_.RetrieveAttribute< MaterialAttribute >() )
        return ( 1 - dotationCategory.GetUrbanAttritionModifer( materialAttribute->GetMaterial().GetId() ) ) * urbanObject_.GetOccupation();
    return 0.;
}
