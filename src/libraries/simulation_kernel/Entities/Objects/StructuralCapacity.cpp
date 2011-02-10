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
#include "UrbanType.h"
#include "MT_Tools/MT_Ellipse.h"
#include "Tools/MIL_Geometry.h"
#include "protocol/ClientSenders.h"
#include <geometry/Types.h>
#include <urban/PhysicalAttribute.h>
#include <urban/StaticModel.h>
#include <urban/MaterialCompositionType.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( StructuralCapacity )

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity()
    : structuralState_    ( 100 )
    , lastStructuralState_( std::numeric_limits< unsigned int >::max() )
    , materialType_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( xml::xistream& xis )
    : structuralState_    ( xis.attribute< unsigned int >( "value" ) )
    , lastStructuralState_( std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: JSR 2010-06-23
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity( const StructuralCapacity& from )
    : structuralState_    ( from.structuralState_ )
    , lastStructuralState_( std::numeric_limits< unsigned int >::max() )
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
    structuralState_ = xis.attribute< unsigned int >( "value" );
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
    if( UrbanObjectWrapper* wrapper = dynamic_cast< UrbanObjectWrapper* >( &object ) )
        if( const urban::PhysicalAttribute* pPhysical = wrapper->GetObject().Retrieve< urban::PhysicalAttribute >() )
            if( pPhysical->GetArchitecture() )
                if( urban::MaterialCompositionType* material = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( pPhysical->GetArchitecture()->GetMaterial() ) )
                    object.GetAttribute< MaterialAttribute >() = MaterialAttribute( *material );
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
        const unsigned int oldStructuralState = structuralState_;
        double attrition = dotation.GetAttrition(  materialAttribute->GetMaterial().GetId() );
        unsigned int damage = static_cast< unsigned int >( 100. * ratio * attrition );
        if( damage <= structuralState_ )
            structuralState_ -= damage;
        else
            structuralState_ = 0;
        const float delta = 0.01f * ( oldStructuralState - structuralState_ );
        if ( ( 1 - MIL_Random::rand_io( MIL_Random::eFire ) ) <= delta )
            for ( IT_Agents it = agents_.begin(); it != agents_.end(); ++it )
                ( *it )->GetRole< PHY_RoleInterface_Composantes >().ApplyUrbanObjectCrumbling( object );
        object.ApplyStructuralState( structuralState_ );
    }
    else
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
    // $$$$  SLG 2010-07-22: TODO Dans le cas où ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des dégats.
    if( !materialAttribute )
        return PHY_ComposanteState::undamaged_;
    const urban::MaterialCompositionType& material = materialAttribute->GetMaterial();
    const urban::MaterialCompositionType::AttritionData* attrition = material.FindAttrition( targetProtection.GetName() );
    if( !attrition )
        throw std::exception( "Error in searching protection" );
    // Tirage de l'état opérationnel
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
    // $$$$  SLG 2010-07-22: TODO Dans le cas où ce n'est pas un bloc urbain (objet, ou quartier/ville), voir comment appliquer des dégats.
    if( area == 0 || materialAttribute == 0 )
        return;
    const double modifier = dotation.GetAttrition( materialAttribute->GetMaterial().GetId() );
    unsigned int damage = static_cast< unsigned int >( 100. * modifier / area );
    if( damage <= structuralState_ )
        structuralState_ -= damage;
    else
        structuralState_ = 0;
    object.ApplyStructuralState( structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::SendState
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
void StructuralCapacity::SendState( sword::UrbanAttributes& message ) const
{
    if( structuralState_ != lastStructuralState_ )
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
    message.mutable_structure()->set_state( structuralState_ );

}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::GetStructuralState
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
unsigned int StructuralCapacity::GetStructuralState() const
{
    return structuralState_;
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::SetStructuralState
// Created: SLG 2010-12-22
// -----------------------------------------------------------------------------
void StructuralCapacity::SetStructuralState( int state )
{
    structuralState_ = state;
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
