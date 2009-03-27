// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InterferenceCapacity.h"
#include "Object.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"

// #include "InterferenceAttribute.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( InterferenceCapacity, "InterferenceCapacity" )

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InterferenceCapacity::InterferenceCapacity( xml::xistream& /*xis*/ )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InterferenceCapacity::InterferenceCapacity()    
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: InterferenceCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InterferenceCapacity::InterferenceCapacity( const InterferenceCapacity& /*from*/ )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
InterferenceCapacity::~InterferenceCapacity()
{

}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void InterferenceCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );            
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void InterferenceCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void InterferenceCapacity::Instanciate( Object& object ) const
{    
    InterferenceCapacity* capacity = new InterferenceCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity::ProcessAgentInside
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void InterferenceCapacity::ProcessAgentInside( Object& object, MIL_Agent_ABC& agent )
{
    if( object.GetArmy().IsAnEnemy( agent.GetArmy() ) == eTristate_True )
        agent.GetRole< PHY_RoleInterface_Communications >().Jam( object );
}

// -----------------------------------------------------------------------------
// Name: InterferenceCapacity::ProcessAgentExiting
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void InterferenceCapacity::ProcessAgentExiting( Object& object, MIL_Agent_ABC& agent )
{    
    agent.GetRole< PHY_RoleInterface_Communications >().Unjam( object );
}