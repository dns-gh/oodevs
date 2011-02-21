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
#include "PHY_MaterialCompositionType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "MT_Tools/MT_Ellipse.h"
#include "Tools/MIL_Geometry.h"
#include "protocol/ClientSenders.h"
#include <geometry/Types.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( StructuralCapacity )

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity()
    : structuralState_    ( 1.f )
    , lastStructuralState_( -1.f )
    , materialType_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( xml::xistream& xis )
    : structuralState_    ( 0.01f * xis.attribute< unsigned int >( "value" ) )
    , lastStructuralState_( -1.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( const StructuralCapacity& from )
    : structuralState_    ( from.structuralState_ )
    , lastStructuralState_( -1.f )
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
// Name: StructuralCapacity::Update
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void StructuralCapacity::Update( xml::xistream& xis, const MIL_Object_ABC& object )
{
    structuralState_ = xis.attribute< unsigned int >( "value" ) * 0.01f;
    object.ApplyStructuralState( structuralState_ );
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
    object.ApplyStructuralState( structuralState_ );
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
    object.ApplyStructuralState( structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyIndirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyIndirectFire( MIL_Object_ABC& object, const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation )
{
    const double objectArea = object.GetLocalisation().GetArea();
    if( !objectArea )
        return;
    if( const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >() )
    {
        const double ratio = MIL_Geometry::IntersectionArea( attritionSurface, object.GetLocalisation() ) / objectArea;
        const float oldStructuralState = structuralState_;
        structuralState_ = std::max( 0., structuralState_ - ratio * dotation.GetAttrition(  materialAttribute->GetMaterial().GetId() ) );
        if ( ( 1 - MIL_Random::rand_io( MIL_Random::eFire ) ) <= oldStructuralState - structuralState_ )
            for ( IT_Agents it = agents_.begin(); it != agents_.end(); ++it )
                ( *it )->GetRole< PHY_RoleInterface_Composantes >().ApplyUrbanObjectCrumbling( object );
        object.ApplyStructuralState( structuralState_ );
    }
    else
        // $$$$ JSR 2011-02-17: temporary hack -> Do not destroy districts or cities (UrbanObject without material attribute)
        if( !dynamic_cast< UrbanObjectWrapper* >( &object ) )
        // $$$$ JSR 2010-07-23: if material attribute is not present, just destroy object?
            object().Destroy();
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ComputeComposanteState
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
const PHY_ComposanteState& StructuralCapacity::ComputeComposanteState( const MIL_Object_ABC& object, const PHY_Protection& targetProtection )
{
    const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >();
    // $$$$  SLG 2010-07-22: TODO Dans le cas o� ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des d�gats.
    if( !materialAttribute )
        return PHY_ComposanteState::undamaged_;
    const PHY_MaterialCompositionType& material = materialAttribute->GetMaterial();
    const PHY_MaterialCompositionType::AttritionData* attrition = material.FindAttrition( targetProtection.GetName() );
    if( !attrition )
        throw std::exception( "Error in searching protection" );
    // Tirage de l'�tat op�rationnel
    const double rRand = ( 1 - MIL_Random::rand_io( MIL_Random::eFire ) );
    const double destruction = attrition->destruction_;
    const double repairableWithEvac = attrition->repairableWithEvac_ + destruction;
    const double repairableNoEvac = attrition->repairableNoEvac_ + repairableWithEvac;
    return rRand <= destruction         ? PHY_ComposanteState::dead_:
           rRand <= repairableWithEvac  ? PHY_ComposanteState::repairableWithEvacuation_:
           rRand <= repairableNoEvac    ? PHY_ComposanteState::repairableWithoutEvacuation_:
                                          PHY_ComposanteState::undamaged_;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyDirectFire
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyDirectFire( const MIL_Object_ABC& object, const PHY_DotationCategory& dotation )
{
    const double area = object.GetLocalisation().GetArea();
    const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >();
    // $$$$  SLG 2010-07-22: TODO Dans le cas o� ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des d�gats.
    if( area == 0 || materialAttribute == 0 )
        return;
    structuralState_ = std::max( 0., structuralState_ - dotation.GetAttrition( materialAttribute->GetMaterial().GetId() ) / area );
    object.ApplyStructuralState( structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::SendState
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
void StructuralCapacity::SendState( sword::UrbanAttributes& message ) const
{
    if( std::abs( structuralState_ - lastStructuralState_ ) >= 0.01f )
    {
        SendFullState( message );
        lastStructuralState_ = structuralState_;
    }
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::SendState
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
void StructuralCapacity::SendFullState( sword::UrbanAttributes& message ) const
{
    message.mutable_structure()->set_state( static_cast< unsigned int>( 100.f * structuralState_ + 0.5f ) );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::OnUpdate
// Created: JSR 2011-02-14
// -----------------------------------------------------------------------------
void StructuralCapacity::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
        structuralState_ = attribute.list( 1 ).areal();
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
