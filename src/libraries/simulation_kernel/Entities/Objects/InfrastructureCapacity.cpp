// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InfrastructureCapacity.h"
#include "StructuralCapacity.h"
#include "ResourceNetworkCapacity.h"
#include "MIL_Object_ABC.h"
#include "protocol/ClientSenders.h"
#include "Urban/PHY_InfrastructureType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InfrastructureCapacity )

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity()
    : functionalState_( 1.f )
    , threshold_      ( 0.3f )
    , enabled_        ( true )
    , needUpdate_     ( true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( xml::xistream& xis )
    : functionalState_( 1.f )
    , threshold_      ( 0.3f )
    , enabled_        ( true )
    , needUpdate_     ( true )
{
    xis >> xml::attribute( "role", role_ )
        >> xml::attribute( "enabled", enabled_ )
        >> xml::attribute( "threshold", threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( const PHY_InfrastructureType& type )
    : functionalState_( 1.f )
    , role_           ( type.GetName() )
    , threshold_      ( 0.3f )
    , enabled_        ( true )
    , needUpdate_     ( true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity constructor
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( const InfrastructureCapacity& from )
    : functionalState_( from.functionalState_ )
    , role_( from.role_ )
    , threshold_( from.threshold_ )
    , enabled_( from.enabled_ )
    , needUpdate_( from.needUpdate_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity destructor
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::~InfrastructureCapacity()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::serialize
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
template< typename Archive >
void InfrastructureCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & functionalState_;
    file & role_;
    file & enabled_;
    file & threshold_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::WriteUrban
// Created: NPT 2012-09-11
// -----------------------------------------------------------------------------
void InfrastructureCapacity::WriteUrban( xml::xostream& xos ) const
{
    xos << xml::start( "infrastructures" );
    xos     << xml::start( "infrastructure" )
                << xml::attribute( "role", role_ )
                << xml::attribute( "enabled", enabled_ )
                << xml::attribute( "threshold", threshold_ )
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::Register
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::Instanciate
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new InfrastructureCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::Update
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    // $$$$ _RC_ SLG 2011-01-19: trouver un moyen pour mettre les valeurs des capacités StructuralCapacity et ResourceNetworkCapacity dans des attributs
    float previousFunctionalState = functionalState_;
    StructuralCapacity* capacity = object.Retrieve< StructuralCapacity >();
    functionalState_ = 1.f;
    if( capacity )
        functionalState_ *= capacity->GetStructuralState();
    ResourceNetworkCapacity* resourceCapacity = object.Retrieve< ResourceNetworkCapacity >();
    if( resourceCapacity )
        functionalState_ *= resourceCapacity->GetFunctionalState();
    if( previousFunctionalState != functionalState_ )
        needUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::Update
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::Update( xml::xistream& xis, const MIL_Object_ABC& /*object*/ )
{
    xis >> xml::optional >> xml::attribute( "enabled", enabled_ )
        >> xml::optional >> xml::attribute( "threshold", threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::SendState
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::SendState( sword::UrbanAttributes& message ) const
{
    if( needUpdate_ )
        SendFullState( message );
    needUpdate_ = false;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::SendFullState
// Created: SLG 2011-01-19
// -----------------------------------------------------------------------------
void InfrastructureCapacity::SendFullState( sword::UrbanAttributes& message ) const
{
    message.mutable_infrastructures()->mutable_infrastructure()->set_type( role_ );
    message.mutable_infrastructures()->mutable_infrastructure()->set_active( IsActive() );
    message.mutable_infrastructures()->mutable_infrastructure()->set_threshold( threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::OnUpdate
// Created: JSR 2011-02-14
// -----------------------------------------------------------------------------
void InfrastructureCapacity::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        needUpdate_ = true;
        if( attribute.list( 1 ).has_booleanvalue() )
            enabled_ = attribute.list( 1 ).booleanvalue();
        else if( attribute.list( 1 ).has_areal() )
            threshold_ = attribute.list( 1 ).areal();
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::IsActive
// Created: JSR 2011-02-08
// -----------------------------------------------------------------------------
bool InfrastructureCapacity::IsActive() const
{
    return enabled_ && functionalState_ >= threshold_;
}
