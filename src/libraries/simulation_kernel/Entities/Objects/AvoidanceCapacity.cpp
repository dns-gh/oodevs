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
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_DynamicData.h"
#include "Object.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( AvoidanceCapacity, "AvoidanceCapacity" )

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AvoidanceCapacity::AvoidanceCapacity( xml::xistream& xis )
    : distance_ ( MIL_Tools::ConvertMeterToSim( xml::attribute( xis, "distance", 0. ) ) )
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
// Name: AvoidanceCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void AvoidanceCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & avoid_
         & distance_;
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Instanciate( Object& object ) const
{    
    object.AddCapacity( new AvoidanceCapacity( *this ) );        
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Finalize
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::Finalize( Object& object )
{
    InitializeArea( object );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::GetAvoidanceLocalisation
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
const TER_Localisation& AvoidanceCapacity::GetLocalisation() const
{
    return avoid_;
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::Initialize
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::InitializeArea( Object& object )
{
    if ( distance_ <= 0. )
        avoid_.Reset();
    else
        ResetDynamicData( object );
}

// -----------------------------------------------------------------------------
// Name: AvoidanceCapacity::ResetDynamicData
// Created: JCR 2008-08-11
// -----------------------------------------------------------------------------
void AvoidanceCapacity::ResetDynamicData( Object& object )
{
    avoid_.Reset( object.GetLocalisation() );
    avoid_.Scale( distance_ );

    const T_PointVector& points = avoid_.GetPoints();
    assert( points.size() > 3 );

    handler_.Reset( new TER_DynamicData( points, TER_PathFindManager::DefaultTerrainData() ) );    
}