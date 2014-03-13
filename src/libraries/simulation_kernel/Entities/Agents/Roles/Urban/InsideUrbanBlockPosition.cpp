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
#include "DefaultUrbanLocationComputer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Objects/MaterialAttribute.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "MT_Tools/MT_Ellipse.h"
#include "Tools/MIL_Geometry.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/UrbanPhysicalCapacity.h"
#pragma warning( push, 0 )
#pragma warning( disable: 4702 )
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#pragma warning( pop )

namespace bg = boost::geometry;
using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition constructor
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
InsideUrbanBlockPosition::InsideUrbanBlockPosition( const MIL_UrbanObject_ABC& urbanObject )
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
MT_Vector2D InsideUrbanBlockPosition::GetFirerPosition( const MT_Vector2D& target, const MT_Vector2D& shooter ) const
{
    TER_DistanceLess cmp ( target );
    T_PointSet collisions( cmp );
    if( !urbanObject_.GetLocalisation().Intersect2D( MT_Line( target, shooter ), collisions, 0 ) )
        return shooter; //// $$$$ _RC_ SBO 2010-07-07: devrait etre throw MASA_EXCEPTION( "error in urbanBlock intersection for firer" );
    return *collisions.begin(); // Nearest point from targetResult
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::GetTargetPosition
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
MT_Vector2D InsideUrbanBlockPosition::GetTargetPosition( MIL_Agent_ABC& firer, UrbanLocationComputer_ABC::Results& targetResult ) const
{
    DefaultUrbanLocationComputer firerComputer;
    firer.Execute< UrbanLocationComputer_ABC >( firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer.Result();
    TER_DistanceLess cmp ( firerResult.position_ );
    T_PointSet collisions( cmp );
    urbanObject_.GetLocalisation().Intersect2D( MT_Line( targetResult.position_, firerResult.position_ ), collisions, 0 );
    if( collisions.size() < 2 )
        return targetResult.position_;  // $$$$ _RC_ SBO 2010-07-07: devrait etre throw MASA_EXCEPTION( " error in urbanBlock intersection for target" );
    MT_Vector2D vector( *collisions.rbegin() - *collisions.begin() );
    vector *= ( 1 - targetResult.urbanDeployment_ );
    const MT_Vector2D pM = *collisions.begin() + vector;
    return pM + ( *collisions.rbegin() - pM );
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
double InsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const MT_Ellipse& attritionSurface ) const
{
    TER_Localisation attritionPolygon( attritionSurface );
    return MIL_Geometry::IntersectionArea( attritionSurface, urbanObject_.GetLocalisation() ) * result.urbanDeployment_;
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeRatioPionInside
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
double InsideUrbanBlockPosition::ComputeRatioPionInside( UrbanLocationComputer_ABC::Results& result, const TER_Polygon& polygon, double modicator ) const
{
    if( modicator > result.urbanDeployment_ ) // SLG : permet d'éviter des incohérence dans la percpetion d'unité quand la cible passe en état posté.
        return polygon.IsInside( result.position_, 0 ) ? 1.0 : 0.0;
    else
    {
        double urbanObjectArea = urbanObject_.GetLocalisation().GetArea();
        if( urbanObjectArea )
        {
            double intersectArea = MIL_Geometry::IntersectionArea( TER_Localisation( polygon ), urbanObject_.GetLocalisation() );
            return ( intersectArea / urbanObjectArea ) * result.urbanDeployment_;
        }
        return 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: InsideUrbanBlockPosition::ComputeUrbanProtection
// Created: SLG 2010-04-27
// -----------------------------------------------------------------------------
double InsideUrbanBlockPosition::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    if( const MaterialAttribute* materialAttribute = urbanObject_.RetrieveAttribute< MaterialAttribute >() )
        if( const UrbanPhysicalCapacity* physical = urbanObject_.Retrieve< UrbanPhysicalCapacity >() )
        {
            const StructuralCapacity* structural = urbanObject_.Retrieve< StructuralCapacity >();
            float structuralState = structural ? structural->GetStructuralState() : 1.f;
            return dotationCategory.GetUrbanAttritionScore( materialAttribute->GetMaterial() ) * physical->GetOccupation() * structuralState;
        }
    return 0.;
}
