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
#include "urban/InfrastructureType.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( InfrastructureCapacity )

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity()
: functionalState_( 100 )
, enabled_( true )
, threshold_( 0 )
, needUpdate_( true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( xml::xistream& xis )
    : functionalState_( 100 )
    , enabled_( true )
    , threshold_( 0 )
    , needUpdate_( true )
{
    InitializeData( xis );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::InfrastructureCapacity
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( const urban::InfrastructureType& type )
    : functionalState_( 100 )
    , role_( type.GetName() )
    , enabled_( true )
    , threshold_( 0 )
    , needUpdate_( true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity constructor
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
InfrastructureCapacity::InfrastructureCapacity( const InfrastructureCapacity& from )
{
    functionalState_ = from.functionalState_;
    role_ = from.role_;
    enabled_ = from.enabled_;
    threshold_ = from.threshold_;
    needUpdate_ = from.needUpdate_;
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
// Name: InfrastructureCapacity::InitializeSpeed
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::InitializeData( xml::xistream& xis )
{
    xis >> xml::attribute( "role", role_ )
        >> xml::attribute( "enabled", enabled_ )
        >> xml::attribute( "threshold", threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::serialize
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
template< typename Archive >
void InfrastructureCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & functionalState_
         & role_
         & enabled_
         & threshold_;
         & needUpdate_;
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
    StructuralCapacity* capacity = object.Retrieve< StructuralCapacity >(); 
    if( capacity )
        functionalState_ = capacity->GetStructuralState();
    ResourceNetworkCapacity* resourceCapacity = object.Retrieve< ResourceNetworkCapacity >(); 
    if( resourceCapacity )
        functionalState_ *= resourceCapacity->GetNetworkState();
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::Update
// Created: SLG 2010-01-13
// -----------------------------------------------------------------------------
void InfrastructureCapacity::Update( xml::xistream& xis, const MIL_Object_ABC& /*object*/ )
{
    InitializeData( xis );
    /*InfrastructureAttribute* infrastructureAttribute = new InfrastructureAttribute( xis );
    const_cast< MIL_Object_ABC& >( object ).tools::Extendable< ObjectAttribute_ABC >::Attach< ObjectAttribute_ABC >( *infrastructureAttribute );
    const_cast< MIL_Object_ABC& >( object ).GetAttribute< InfrastructureAttribute >() = *infrastructureAttribute;*/
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
    message.mutable_infrastructures()->mutable_infrastructure()->set_active( enabled_ );
    message.mutable_infrastructures()->mutable_infrastructure()->set_threshold( threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::SetEnabled
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void InfrastructureCapacity::SetEnabled( bool enabled )
{
    enabled_ = enabled;
    needUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureCapacity::SetThreshold
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void InfrastructureCapacity::SetThreshold( float threshold )
{
    threshold_ = threshold;
    needUpdate_ = true;
}
