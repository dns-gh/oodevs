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
#include "MaterialAttribute.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "geometry/Types.h"
#include "tools/MIL_Geometry.h"
#include "protocol/ClientSenders.h"
#include <urban/Architecture.h>
#include <urban/StaticModel.h>
#include <urban/MaterialCompositionType.h>
#include <urban/TerrainObject_ABC.h>
#include <UrbanType.h>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( StructuralCapacity )

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity()
    : structuralState_( 1 )
    , materialType_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( xml::xistream& xis )
{
    Load( xis );
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
// Name: StructuralCapacity::Load
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void StructuralCapacity::Load( xml::xistream& xis )
{
    structuralState_ = 0.01f * xis.attribute< int >( "value" );
}

// -----------------------------------------------------------------------------
// Name: void StructuralCapacity::serialize
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
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::Instanciate
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
void StructuralCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    StructuralCapacity* capacity = new StructuralCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC* >( capacity ) );
    UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( wrapper )
    {
        const urban::Architecture* architecture = wrapper->GetObject().RetrievePhysicalFeature< urban::Architecture >();
        if( architecture )
        {
            urban::MaterialCompositionType* material = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() );
            object.GetAttribute< MaterialAttribute >() = MaterialAttribute( *material );
        }
    }
}

namespace
{
    geometry::Polygon2f EllipseToPolygon( const MT_Ellipse& ellipse )
    {
        geometry::Polygon2f polygon;
        polygon.Add( geometry::Point2f( float( ellipse.GetMajorAxisHighPoint().rX_ + ellipse.GetMinorAxisHighPoint().rX_ - ellipse.GetCenter().rX_ ), 
            float( ellipse.GetMajorAxisHighPoint().rY_ + ellipse.GetMinorAxisHighPoint().rY_ - ellipse.GetCenter().rY_ ) ) );
        polygon.Add( geometry::Point2f( float( ellipse.GetMajorAxisHighPoint().rX_ - ellipse.GetMinorAxisHighPoint().rX_ + ellipse.GetCenter().rX_ ),
            float( ellipse.GetMajorAxisHighPoint().rY_ - ellipse.GetMinorAxisHighPoint().rY_ + ellipse.GetCenter().rY_ ) ) );
        polygon.Add( geometry::Point2f( float( 3 * ellipse.GetCenter().rX_ - ellipse.GetMajorAxisHighPoint().rX_ - ellipse.GetMinorAxisHighPoint().rX_ ),
            float( 3 * ellipse.GetCenter().rY_ - ellipse.GetMajorAxisHighPoint().rY_ - ellipse.GetMinorAxisHighPoint().rY_ ) ) );
        polygon.Add( geometry::Point2f( float( ellipse.GetCenter().rX_ - ellipse.GetMajorAxisHighPoint().rX_ + ellipse.GetMinorAxisHighPoint().rX_ ),
            float( ellipse.GetCenter().rY_ - ellipse.GetMajorAxisHighPoint().rY_ + ellipse.GetMinorAxisHighPoint().rY_ ) ) );
        return polygon;
    }
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyIndirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyIndirectFire( MIL_Object_ABC& object, const MT_Ellipse& attritionSurface, const PHY_DotationCategory& dotation )
{
    const float objectArea = static_cast< float >( object.GetLocalisation().GetArea() );
    if( !objectArea )
        return;
    geometry::Polygon2f attritionPolygon = EllipseToPolygon( attritionSurface );
    CT_PointVector points = object.GetLocalisation().GetPoints();
    geometry::Polygon2f p;
    for( T_PointVector::const_iterator it = points.begin(); it != points.end(); ++it )
        p.Add( geometry::Point2f( static_cast< float >( it->rX_ ), static_cast< float >( it->rY_ ) ) );
    float ratio = MIL_Geometry::IntersectionArea( attritionPolygon, p ) / objectArea;

    float oldStructuralState = structuralState_;
    const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >();
    if( materialAttribute )
    {
        structuralState_ -= ratio * float( dotation.GetAttrition(  materialAttribute->GetMaterial().GetId()) );
        if( structuralState_ < 0 )
            structuralState_ = 0;

        float delta = oldStructuralState - structuralState_;
        if ( MIL_Random::rand_io( MIL_Random::eFire ) <= delta )
        {
            for ( IT_Agents it = agents_.begin(); it != agents_.end(); ++it )
                ( *it )->GetRole< PHY_RoleInterface_Composantes >().ApplyUrbanObjectCrumbling( object );
        }
    }
    else
    {
        // $$$$ JSR 2010-07-23: if material attribute is not present, just destroy object?
        object().Destroy();
    }
}
// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ComputeComposanteState
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
const PHY_ComposanteState& StructuralCapacity::ComputeComposanteState( const MIL_Object_ABC& object, const PHY_Protection& targetProtection )
{
    const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >();
    // $$$$  SLG 2010-07-22: TODO Dans le cas où ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des dégats.
    if( !materialAttribute )
        return PHY_ComposanteState::undamaged_;

    urban::MaterialCompositionType& material = materialAttribute->GetMaterial();
    urban::MaterialCompositionType::AttritionData* attrition = material.FindAttrition( targetProtection.GetName() );
    if( !attrition ) 
        throw std::exception( "Error in searching protection" );
    // Tirage de l'état opérationnel
    MT_Float rRand = MIL_Random::rand_io( MIL_Random::eFire );

    double destruction = attrition->destruction_;
    double repairableWithEvac = attrition->repairableWithEvac_ + destruction;
    double repairableNoEvac = attrition->repairableNoEvac_ + repairableWithEvac;

    return rRand <= destruction         ? PHY_ComposanteState::dead_:
           rRand <= repairableWithEvac  ? PHY_ComposanteState::repairableWithEvacuation_   :
           rRand <= repairableNoEvac    ? PHY_ComposanteState::repairableWithoutEvacuation_:
                                          PHY_ComposanteState::undamaged_;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyDirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyDirectFire( const MIL_Object_ABC& object, const PHY_DotationCategory& dotation )
{
    const float area = static_cast< float >( object.GetLocalisation().GetArea() );
    if( area )
    {
        const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >();
        // $$$$  SLG 2010-07-22: TODO Dans le cas où ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des dégats.
        if( !materialAttribute )
            return;
        const float modifier = static_cast< float >( dotation.GetAttrition( materialAttribute->GetMaterial().GetId() ) );
        structuralState_-= modifier / area;
    }
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::SendState
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
void StructuralCapacity::SendState( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    message.mutable_capacity()->set_structuralstate( structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::GetStructuralState
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
float StructuralCapacity::GetStructuralState() const
{
    return structuralState_;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ProcessAgentEntering
// Created: SLG 2010-06-29
// -----------------------------------------------------------------------------
void StructuralCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agents_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ProcessAgentExiting
// Created: SLG 2010-06-29
// -----------------------------------------------------------------------------
void StructuralCapacity::ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agents_.erase( std::find( agents_.begin(), agents_.end(), &agent ) );
}
