// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "StructuralCapacity.h"
#include "MIL_Object_ABC.h"
#include "UrbanType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "geometry/Types.h"
#include "tools/MIL_Geometry.h"
#include "urban/Architecture.h"
#include "urban/TerrainObject_ABC.h"
#include "urban/StaticModel.h"
#include "urban/MaterialCompositionType.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( StructuralCapacity )

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity()
    : structuralState_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( xml::xistream& xis )
    : structuralState_( 0.01f * xml::attribute< int >( xis, "value" ) )
{
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( const StructuralCapacity& from )
    : structuralState_( from.structuralState_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity destructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::~StructuralCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void StructuralCapacity::serialize
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
template< typename Archive >
void StructuralCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
         & structuralState_;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::Register
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
void StructuralCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::Instanciate
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
void StructuralCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    StructuralCapacity* capacity = new StructuralCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyIndirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyIndirectFire( const urban::TerrainObject_ABC& object, const MT_Ellipse& attritionSurface, const PHY_DotationCategory& dotation )
{
    float urbanObjectArea = object.GetFootprint()->ComputeArea();
    if( !urbanObjectArea )
        return;
    geometry::Polygon2f attritionPolygon; 
    attritionPolygon.Add( geometry::Point2f( float( attritionSurface.GetMajorAxisHighPoint().rX_ + attritionSurface.GetMinorAxisHighPoint().rX_ - attritionSurface.GetCenter().rX_ ), 
        float( attritionSurface.GetMajorAxisHighPoint().rY_ + attritionSurface.GetMinorAxisHighPoint().rY_ - attritionSurface.GetCenter().rY_ ) ) );
    attritionPolygon.Add( geometry::Point2f( float( attritionSurface.GetMajorAxisHighPoint().rX_ - attritionSurface.GetMinorAxisHighPoint().rX_ + attritionSurface.GetCenter().rX_ ),
        float( attritionSurface.GetMajorAxisHighPoint().rY_ - attritionSurface.GetMinorAxisHighPoint().rY_ + attritionSurface.GetCenter().rY_ ) ) );
    attritionPolygon.Add( geometry::Point2f( float( 3 * attritionSurface.GetCenter().rX_ - attritionSurface.GetMajorAxisHighPoint().rX_ - attritionSurface.GetMinorAxisHighPoint().rX_ ),
        float( 3 * attritionSurface.GetCenter().rY_ - attritionSurface.GetMajorAxisHighPoint().rY_ - attritionSurface.GetMinorAxisHighPoint().rY_ ) ) );
    attritionPolygon.Add( geometry::Point2f( float( attritionSurface.GetCenter().rX_ - attritionSurface.GetMajorAxisHighPoint().rX_ + attritionSurface.GetMinorAxisHighPoint().rX_ ),
        float( attritionSurface.GetCenter().rY_ - attritionSurface.GetMajorAxisHighPoint().rY_ + attritionSurface.GetMinorAxisHighPoint().rY_ ) ) );
    float ratio = MIL_Geometry::IntersectionArea( attritionPolygon, *object.GetFootprint() ) / urbanObjectArea;

    float modifier = 0.;
    const urban::Architecture* architecture = object.RetrievePhysicalFeature< urban::Architecture >();
    if( architecture )
        modifier = float( dotation.GetAttrition( UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId() ) );

    structuralState_-= ratio * modifier;
    if( structuralState_  < 0 ) 
        structuralState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyDirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyDirectFire( const urban::TerrainObject_ABC& object, const PHY_DotationCategory& dotation )
{
    if( !object.GetFootprint() )
        return;
    float area = object.GetFootprint()->ComputeArea(); 
    if( area )
    {
        float modifier = 0.;
        const urban::Architecture* architecture = object.RetrievePhysicalFeature< urban::Architecture >();
        if( architecture )
            modifier = float( dotation.GetAttrition( UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId() ) );
        structuralState_-= modifier / object.GetFootprint()->ComputeArea();
    }
}
