// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "IntoxicationCapacity.h"
#include "MIL_NbcAgentType.h"
#include "NBCAttribute.h"
#include "MIL_Object_ABC.h"
#include "ToxicAttribute_ABC.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( IntoxicationCapacity )

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity( xml::xistream& xis )
    : maxToxic_ ( xml::attribute< int >( xis, "max-toxic" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity()
    : maxToxic_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity( const IntoxicationCapacity& from )
    : maxToxic_( from.maxToxic_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity destructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::~IntoxicationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void IntoxicationCapacity::serialize
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename Archive >
void IntoxicationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
         & maxToxic_;
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    IntoxicationCapacity* capacity = new IntoxicationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::ProcessAgentInside
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void IntoxicationCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();

    if( pNBC && pNBC->IsPoisonous() )
    {
        const ToxicAttribute_ABC* pAttribute = object.RetrieveAttribute< ToxicAttribute_ABC >();
        if( pAttribute )
        {
            MIL_ToxicEffectManipulator contamination( pAttribute->GetContaminationEffect( *pNBC, position ) );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Poison( contamination );
        }
        else
        {
            MIL_ToxicEffectManipulator contamination( object.GetAttribute< NBCAttribute >().GetNBCAgents(), 1 );
            agent.GetRole< nbc::PHY_RoleInterface_NBC >().Poison( contamination );
        }
    }
}
