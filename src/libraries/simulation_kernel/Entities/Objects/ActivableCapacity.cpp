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
#include "ConstructionAttribute.h"
#include "MIL_Object_ABC.h"
#include "ObstacleAttribute.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ActivableCapacity )

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( xml::xistream& /*xis*/ )
    : timeOfCreation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity()
    : timeOfCreation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( const ActivableCapacity& /*from*/ )
    : timeOfCreation_( 0 )
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
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & timeOfCreation_;
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
    object.GetAttribute< ObstacleAttribute >() = ObstacleAttribute();
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Activate
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
void ActivableCapacity::Activate( MIL_Object_ABC& object )
{
    return object.GetAttribute< ObstacleAttribute >().Activate( true );
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
    if( timeOfCreation_ == 0 )
    {
        if( object.GetAttribute< ConstructionAttribute >().IsConstructed() )
            timeOfCreation_ = time;
    }
    if( timeOfCreation_ != 0 )
    {
        unsigned int delta = time - timeOfCreation_;
        ObstacleAttribute& attr = object.GetAttribute< ObstacleAttribute >();
        if( attr.IsTimesUndefined() )
        {
            if( !attr.IsActivated() && attr.ActivatedAfterConstruction() )
                attr.Activate( true );
        }
        else
        {
            if( attr.GetActivityTime() > 0 && delta >= MIL_Tools::ConvertSecondsToSim( attr.GetEndActivity() ) )
                attr.Activate( false );
            else if( delta >= MIL_Tools::ConvertSecondsToSim( attr.GetActivationTime() ) )
                attr.Activate( true );
        }
    }
}
