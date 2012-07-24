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
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ProtectionCapacity )

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProtectionCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity( xml::xistream& xis )
    : size_max_ ( xis.attribute< int >( "max-size" ) )
    , bGeniePrepared_ ( xis.attribute< bool >( "geniePrepared" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProtectionCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity()
    : size_max_ ( 0 )
    , bGeniePrepared_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ProtectionCapacity::ProtectionCapacity( const ProtectionCapacity& from )
    : size_max_ ( from.size_max_ )
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
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & size_max_
         & bGeniePrepared_;
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
    IT_AgentContainer it = container_.find( &agent );
    if( it != container_.end() )
    {
        if( bGeniePrepared_ )
            agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
        else
            agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 1. );

        container_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProcessAgentInside
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ProtectionCapacity::ProcessAgentInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    CIT_AgentContainer it = container_.find( &agent );
    DEC_RolePion_Decision* decision = agent.RetrieveRole< DEC_RolePion_Decision >();
    E_OperationalState opState = eOpStateOperational;
    if( decision )
        opState = decision->GetOperationalState();
    if( it != container_.end() )
    {
        if( opState != eOpStateOperational )
            container_.erase( it );
        else
        {
            if( bGeniePrepared_ )
                agent.GetRole< PHY_RoleInterface_Posture >().SetPosturePostePrepareGenie();
            else
                agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 2. );
        }
    }
    else if( opState == eOpStateOperational && static_cast< int >( container_.size() ) < size_max_ )
        container_.insert( &agent );
}

// -----------------------------------------------------------------------------
// Name: ProtectionCapacity::ProcessAgentEntering
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void ProtectionCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    CIT_AgentContainer it = container_.find( &agent );
    if( it == container_.end() && static_cast< int >( container_.size() ) < size_max_ )
        container_.insert( &agent );
}
