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
#include "Object.h"
#include "ToxicAttribute_ABC.h"
#include "MIL_ToxicEffectManipulator.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( IntoxicationCapacity, "IntoxicationCapacity" )

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
IntoxicationCapacity::IntoxicationCapacity( xml::xistream& xis )
    : type_ ( xml::attribute< std::string >( xis, "type" ) )
    , maxToxic_ ( xml::attribute< int >( xis, "max-toxic" ) )
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
    : type_( from.type_ )
    , maxToxic_( from.maxToxic_ )
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
void IntoxicationCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
         & type_
         & maxToxic_;
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void IntoxicationCapacity::Instanciate( Object& object ) const
{    
    IntoxicationCapacity* capacity = new IntoxicationCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: IntoxicationCapacity::ProcessAgentInside
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void IntoxicationCapacity::ProcessAgentInside( Object& object, MIL_Agent_ABC& agent )
{
    const MT_Vector2D& position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const NBCAttribute* pNBC = object.RetrieveAttribute< NBCAttribute >();

    if( pNBC && pNBC->IsPoisonous() )
    {
        const ToxicAttribute_ABC* pAttribute = object.RetrieveAttribute< ToxicAttribute_ABC >();
        if( pAttribute )
        {
            MIL_ToxicEffectManipulator contamination( pAttribute->GetContaminationEffect( *pNBC, position ) );
            agent.GetRole< PHY_RoleInterface_NBC >().Poison( contamination );
        }
        else
        {
            MIL_ToxicEffectManipulator contamination( object.GetAttribute< NBCAttribute >().GetNBCAgents(), 1 );
            agent.GetRole< PHY_RoleInterface_NBC >().Poison( contamination );
        }
    }
}
