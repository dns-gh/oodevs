// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AvoidanceCapacity.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_DynamicData.h"
#include "MIL_Object_ABC.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AvoidanceCapacity )

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AvoidanceCapacity::AvoidanceCapacity( xml::xistream& xis )
    : distance_ ( MIL_Tools::ConvertMeterToSim( xis.attribute< float >( "distance", 0. ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AvoidanceCapacity::AvoidanceCapacity()
    : distance_ ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AvoidanceCapacity::~AvoidanceCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity constructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
AvoidanceCapacity::AvoidanceCapacity( const AvoidanceCapacity& from )
    : avoid_ ( from.avoid_ )
    , distance_ ( from.distance_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::load
// Created: JSR 2012-04-16
// -----------------------------------------------------------------------------
void AvoidanceCapacity::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file >> avoid_;
    file >> distance_;
    const T_PointVector& points = avoid_.GetPoints();
    if( points.size() > 3 )
        data_ = CreateAndRegisterDynamicData( points, "" );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::save
// Created: JSR 2012-04-16
// -----------------------------------------------------------------------------
void AvoidanceCapacity::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file << avoid_;
    file << distance_;
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new AvoidanceCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Finalize
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Finalize( MIL_Object_ABC& object )
{
    InitializeArea( object );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::GetLocalisation
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
const TER_Localisation& AvoidanceCapacity::GetLocalisation() const
{
    return avoid_;
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::GetDistance
// Created: CMA 2012-04-25
// -----------------------------------------------------------------------------
const double AvoidanceCapacity::GetDistance() const
{
    return distance_;
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Initialize
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::InitializeArea( MIL_Object_ABC& object )
{
    if( distance_ <= 0. )
        avoid_.Reset();
    else
        ResetDynamicData( object );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::ResetDynamicData
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::ResetDynamicData( MIL_Object_ABC& object )
{
    avoid_.Reset( object.GetLocalisation() );
    avoid_.Scale( distance_ );

    const T_PointVector& points = avoid_.GetPoints();
    if( points.size() > 3 )
        data_ = CreateAndRegisterDynamicData( points, "" );
}
