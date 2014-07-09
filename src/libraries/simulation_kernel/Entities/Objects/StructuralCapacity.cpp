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
#include "MIL_AgentServer.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MineAttribute.h"
#include "MIL_Random.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "protocol/ClientSenders.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "simulation_terrain/TER_Geometry.h"
#include <geometry/Types.h>
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( StructuralCapacity )

// -----------------------------------------------------------------------------
// Name: StructuralCapacity constructor
// Created: SLG 2010-06-17
// -----------------------------------------------------------------------------
StructuralCapacity::StructuralCapacity()
    : structuralState_    ( 1.f )
    , lastStructuralState_( -1.f )
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
// Name: StructuralCapacity::WriteUrban
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
void StructuralCapacity::WriteUrban( xml::xostream& xos ) const
{
    if( structuralState_ != 1.f )
        xos << xml::start( "structural-state" )
                << xml::attribute( "value", static_cast< unsigned int >( 100 * structuralState_ + 0.5f ) )
            << xml::end;
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
void StructuralCapacity::ApplyIndirectFire( MIL_Object_ABC& object, const TER_Localisation& attritionSurface,
                                            const PHY_DotationCategory& dotation, MIL_Army_ABC* army )
{
    if( const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >() )
    {
        if( ApplyDestruction( object, attritionSurface, dotation.GetUrbanAttritionScore( materialAttribute->GetMaterial() ) ) && army )
            CreateCrumbling( object, attritionSurface, *army );
    }
    else
        // $$$$ JSR 2011-02-17: temporary hack -> Do not destroy districts or cities (UrbanObject without material attribute)
        if( !dynamic_cast< MIL_UrbanObject_ABC* >( &object ) )
        // $$$$ JSR 2010-07-23: if material attribute is not present, just destroy object?
            object().Destroy();
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyDestruction
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
void StructuralCapacity::ApplyDestruction( MIL_Object_ABC& object, const TER_Localisation& attritionSurface,
                                           const PHY_UrbanAttritionData& attrition )
{
    if( const MaterialAttribute* materialAttribute = object.RetrieveAttribute< MaterialAttribute >() )
        ApplyDestruction( object, attritionSurface, attrition.GetScore( materialAttribute->GetMaterial() ) );
    else
        // $$$$ JSR 2011-02-17: temporary hack -> Do not destroy districts or cities (UrbanObject without material attribute)
        if( !dynamic_cast< MIL_UrbanObject_ABC* >( &object ) )
        // $$$$ JSR 2010-07-23: if material attribute is not present, just destroy object?
            object().Destroy();
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::ApplyDestruction
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
bool StructuralCapacity::ApplyDestruction( MIL_Object_ABC& object, const TER_Localisation& attritionSurface, double factor )
{
    const double objectArea = object.GetLocalisation().GetArea();
    if( !objectArea )
        return false;

    double ratio = 0.;
    if( attritionSurface.GetType() == TER_Localisation::ePoint || attritionSurface.GetType() == TER_Localisation::eLine )
        ratio = 1.0;
    else
        ratio = TER_Geometry::IntersectionArea( attritionSurface, object.GetLocalisation() ) / objectArea;
    const float oldStructuralState = structuralState_;

    structuralState_ = static_cast< float >( std::max( 0., (double)structuralState_ - ratio * ( 1 - factor ) ) );
    for( auto it = agents_.begin(); it != agents_.end(); ++it )
        if( ( 1. - MIL_Random::rand_io( MIL_Random::eFire ) ) <= oldStructuralState - structuralState_ )
            (*it)->GetRole< PHY_RoleInterface_Composantes >().ApplyUrbanObjectCrumbling( object );
    object.ApplyStructuralState( structuralState_ );
    return oldStructuralState != structuralState_;
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
    const PHY_MaterialCompositionType& material = materialAttribute->GetMaterial();
    const PHY_MaterialCompositionType::AttritionData* attrition = material.FindAttrition( targetProtection.GetName() );
    if( !attrition )
        throw MASA_EXCEPTION( "Error in searching protection" );
    // Tirage de l'état opérationnel
    const double rRand = ( 1. - MIL_Random::rand_io( MIL_Random::eFire ) );
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
    structuralState_ = (float)std::max( 0., (double)structuralState_ - ( 1 - dotation.GetUrbanAttritionScore( materialAttribute->GetMaterial() ) ) / area );
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
    message.mutable_structure()->set_state( static_cast< unsigned int >( 100.f * structuralState_ + 0.5f ) );
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
    boost::remove_erase( agents_, &agent );
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::Build
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
void StructuralCapacity::Build( double rDeltaPercentage )
{
    structuralState_ = std::max( 0.f, std::min( 1.f, structuralState_ + static_cast< float >( rDeltaPercentage ) ) );
    if( rDeltaPercentage > 0 && structuralState_ > 0.99f )
        structuralState_ = 1.f;
}

namespace
{
    MT_Vector2D CreateRandomVector( MT_Line& line )
    {
        double distance = line.GetPosStart().Distance( line.GetPosEnd() ) / 2.;
        const MT_Vector2D point( line.GetCenter() + ( line.GetPosStart() - line.GetCenter() ).Normalize() * MIL_Random::rand_ii( -distance, distance ) );
        return point;
    }

    T_PointVector CreatePolygon( MT_Line& line )
    {
        double distance = 20.;  // $$$$ _RC_ LGY 2011-08-29: distance hardcoded
        MT_Vector2D vector = CreateRandomVector( line );
        MT_Vector2D vDirection( line.GetPosStart() - vector );
        vDirection.Normalize() *= distance;
        MT_Vector2D rDirection( line.GetPosStart() - vector );
        rDirection.Normalize() *= distance;
        rDirection.Rotate( MT_PI / 2 );
        return boost::assign::list_of< MT_Vector2D >( vector + vDirection )( vector - rDirection )( vector - vDirection )( vector + rDirection );
    }
}

// -----------------------------------------------------------------------------
// Name: StructuralCapacity::CreateCrumbling
// Created: LGY 2011-08-25
// -----------------------------------------------------------------------------
void StructuralCapacity::CreateCrumbling( MIL_Object_ABC& object, const TER_Localisation& surface, MIL_Army_ABC& army ) const
{
    T_PointVector points = object.GetLocalisation().GetPoints();
    if( !points.empty() )
    {
        std::vector< MT_Line > lines;
        T_PointVector landslide;
        CIT_PointVector start = points.begin();
        CIT_PointVector end = points.begin();
        ++end;
        for( ; start != points.end(); ++start, ++end )
        {
            if( end == points.end() )
                end = points.begin();
            if( *start == *end )
                continue;
            MT_Line line( *start, *end );
            if( surface.Intersect2D( line ) )
                lines.push_back( line );
        }
        if( !lines.empty() )
        {
            MIL_Random::random_shuffle( lines );
            TER_Localisation localisation( TER_Localisation::ePolygon, CreatePolygon( lines.front() ) );
            MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( "landslide", &army, localisation );
            if( pObject )
            {
                MineAttribute* mineAttribute = pObject->RetrieveAttribute< MineAttribute >();
                if( mineAttribute )
                    mineAttribute->Set(0.);//default valorization is set to 100%
            }
        }
    }
}
