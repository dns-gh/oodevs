// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ObjectFilter.cpp $
// $Author: Nld $
// $Modtime: 26/10/04 16:00 $
// $Revision: 1 $
// $Workfile: MIL_ObjectFilter.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectFilter.h"
#include "MIL_ObjectType_ABC.h"
#include "AttritionCapacity.h"
#include "AvoidanceCapacity.h"
#include "ContaminationCapacity.h"
#include "DisasterCapacity.h"
#include "InterferenceCapacity.h"
#include "MobilityCapacity.h"
#include "TrafficabilityCapacity.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter constructor
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_ObjectFilter::MIL_ObjectFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter constructor
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
MIL_ObjectFilter::MIL_ObjectFilter( const std::vector< std::string >& params )
{
    for( auto it = params.begin(); it != params.end(); ++it )
        Set( *it );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter destructor
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_ObjectFilter::~MIL_ObjectFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter::Set
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
void MIL_ObjectFilter::Set( const std::string& type )
{
    objects_.insert( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter::Reset
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
void MIL_ObjectFilter::Reset( const std::string& type )
{
    objects_.erase( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectFilter::Test
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
bool MIL_ObjectFilter::Test( const MIL_ObjectType_ABC& type ) const
{
    return objects_.find( type.GetName() ) != objects_.end();
}

bool IsDangerousObject( const MIL_ObjectType_ABC& type )
{
    return type.GetCapacity< AttritionCapacity >()
        || type.GetCapacity< ContaminationCapacity >()
        || type.GetCapacity< InterferenceCapacity >()
        || type.GetCapacity< AvoidanceCapacity >();
}

// -----------------------------------------------------------------------------
// Name: MIL_DangerousObjectFilter::Test
// Created: LDC 2012-02-01
// -----------------------------------------------------------------------------
bool MIL_DangerousObjectFilter::Test( const MIL_ObjectType_ABC& type ) const
{
    return IsDangerousObject( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_PathObjectFilter::Test
// Created: LDC 2013-01-02
// Any object that has an incidence on movement should be tested to true.
// -----------------------------------------------------------------------------
bool MIL_PathObjectFilter::Test( const MIL_ObjectType_ABC& type ) const
{
    return type.GetCapacity< MobilityCapacity >() || type.GetCapacity< TrafficabilityCapacity >() || type.GetCapacity< DisasterCapacity >() || MIL_DangerousObjectFilter::Test( type );
}
