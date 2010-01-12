// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyCapacity.h"
#include "Object.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"

#include "tools/Resolver.h"

#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( SupplyCapacity, "SupplyCapacity" )

// -----------------------------------------------------------------------------
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
SupplyCapacity::SupplyCapacity( xml::xistream& xis )
{
    UNREFERENCED_PARAMETER( xis );
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
SupplyCapacity::SupplyCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
SupplyCapacity::SupplyCapacity( const SupplyCapacity& from )
{
    UNREFERENCED_PARAMETER( from );
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity Destructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
SupplyCapacity::~SupplyCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void SupplyCapacity::serialize
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
template< typename Archive > 
void SupplyCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::Instanciate( Object& object ) const
{    
    SupplyCapacity* capacity = new SupplyCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::ProcessAgentInside
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent )
{
    if( &agent.GetArmy() == &object.GetArmy() )
    {
        MIL_AgentPion* pion = dynamic_cast< MIL_AgentPion* >( &agent );
        if( pion )
        {
            ASN1T_MsgUnitMagicAction asnMsg;
            asnMsg.action.t = T_MsgUnitMagicAction_action_recompletement_ressources;
            //@TODO MGD replace asn by action in general, temp hack with an empty resolver because this magic action dont't use armies
            pion->OnReceiveMsgUnitMagicAction( asnMsg, tools::Resolver< MIL_Army_ABC >() );
        }
    }
}
