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
#include "MIL_Object_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver.h>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( SupplyCapacity )

// -----------------------------------------------------------------------------
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
SupplyCapacity::SupplyCapacity( xml::xistream& /*xis*/ )
{
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
SupplyCapacity::SupplyCapacity( const SupplyCapacity& /*from*/ )
{
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
void SupplyCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    SupplyCapacity* capacity = new SupplyCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyCapacity::ProcessAgentInside
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void SupplyCapacity::ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    if( &agent.GetArmy() == object.GetArmy() )
    {
        MIL_AgentPion* pion = dynamic_cast< MIL_AgentPion* >( &agent );
        if( pion )
        {
            sword::UnitMagicAction msg;
            msg.set_type( sword::UnitMagicAction_Type_recover_resources );
            //@TODO MGD replace msg by action in general, temp hack with an empty resolver because this magic action dont't use armies
            pion->OnReceiveUnitMagicAction( msg, tools::Resolver< MIL_Army_ABC >(), 0, 0 );
        }
    }
}
