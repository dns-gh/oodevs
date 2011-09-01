// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ActivableCapacity.h"
#include "MIL_AgentServer.h"
#include "MIL_Object_ABC.h"
#include "ObstacleAttribute.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ActivableCapacity )

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( xml::xistream& /*xis*/ )
    : timeOfCreation_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity()
    : timeOfCreation_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( const ActivableCapacity& /*from*/ )
    : timeOfCreation_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::~ActivableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::serialize
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
template< typename Archive > void ActivableCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & timeOfCreation_;
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ActivableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ActivableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new ActivableCapacity( *this ) );
    object.GetAttribute< ObstacleAttribute >() = ObstacleAttribute( true ); // reserved obstacle
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Activate
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
void ActivableCapacity::Activate( MIL_Object_ABC& object )
{
    return object.GetAttribute< ObstacleAttribute >().Activate();
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::IsActivate
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
bool ActivableCapacity::IsActivated( MIL_Object_ABC& object ) const
{
    return object.GetAttribute< ObstacleAttribute >().IsActivated();
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Update
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void ActivableCapacity::Update( MIL_Object_ABC& object, unsigned int time )
{
    unsigned int delta = static_cast< int >( time - timeOfCreation_ );
    ObstacleAttribute& attr = object.GetAttribute< ObstacleAttribute >();
    if( attr.GetActivationTime() > 0 && !attr.IsActivated() && delta > MIL_Tools::ConvertSecondsToSim( attr.GetActivationTime() ) )
        attr.Activate();
    if( attr.IsDeactivable() && delta > MIL_Tools::ConvertSecondsToSim( attr.GetEndActivity() ) )
        attr.Deactivate();
}
