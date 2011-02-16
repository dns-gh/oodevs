// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SealOffCapacity.h"
#include "AnimatorAttribute.h"
#include "SealOffAttribute.h"
#include "MIL_Object_ABC.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( SealOffCapacity )

// -----------------------------------------------------------------------------
// Name: SealOffCapacity::SealOffCapacity
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffCapacity::SealOffCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity::SealOffCapacity
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffCapacity::SealOffCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity constructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffCapacity::SealOffCapacity( const SealOffCapacity& /*from*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity destructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffCapacity::~SealOffCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void SealOffCapacity::serialize
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
template< typename Archive > void SealOffCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity::Register
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity::Instanciate
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new SealOffCapacity( *this ) );
    object.GetAttribute< SealOffAttribute >() = SealOffAttribute();
}

// -----------------------------------------------------------------------------
// Name: SealOffCapacity::Update
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    SealOffAttribute& sealOffLevel = object.GetAttribute< SealOffAttribute >();
    AnimatorAttribute& animators = object.GetAttribute< AnimatorAttribute >();

    float currentLevel = 0.f;
    const AnimatorAttribute::T_AgentSet& agents = animators.GetAnimators();
    for( AnimatorAttribute::CIT_AgentSet it = agents.begin(); it != agents.end(); it++ )
    {
        currentLevel += 0.2f;//get capacity
    }
    sealOffLevel.SetSealOffLevel( currentLevel );

}
