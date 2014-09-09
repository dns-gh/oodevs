// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ProtectionCapacity.h"
#include "Object.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ProtectionCapacity )

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProtectionCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity( xml::xistream& xis )
    : maxSize_       ( xis.attribute< std::size_t >( "max-size" ) )
    , bGeniePrepared_( xis.attribute< bool >( "geniePrepared" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProtectionCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity()
    : maxSize_       ( 0 )
    , bGeniePrepared_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity( const ProtectionCapacity& from )
    : maxSize_       ( from.maxSize_ )
    , bGeniePrepared_( from.bGeniePrepared_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::~ProtectionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void ProtectionCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & maxSize_;
    file & agents_;
    file & bGeniePrepared_;
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ProtectionCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ProtectionCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    ProtectionCapacity* capacity = new ProtectionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProcessAgentExiting
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ProtectionCapacity::ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    auto it = agents_.find( &agent );
    if( it == agents_.end() )
        return;
    agents_.erase( it );
    if( bGeniePrepared_ )
        agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
    else
        agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 1. );
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProcessAgentInside
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ProtectionCapacity::ProcessAgentInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    if( agents_.find( &agent ) == agents_.end() )
        return;
    if( bGeniePrepared_ )
        agent.GetRole< PHY_RoleInterface_Posture >().SetPosturePostePrepareGenie();
    else
        agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 2. );
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProcessAgentEntering
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void ProtectionCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    if( agents_.size() < maxSize_ )
        agents_.insert( &agent );
}
