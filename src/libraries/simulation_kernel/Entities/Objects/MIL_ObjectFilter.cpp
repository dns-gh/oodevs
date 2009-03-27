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
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_ObjectFilter::MIL_ObjectFilter( DIA_Parameters& diaParameters, uint nFirstDiaParam )
{
    uint nNbrParams = diaParameters.GetParameters().size();
    for( uint i = nFirstDiaParam; i < nNbrParams; ++i )
        Set( diaParameters.GetParameter( i ).ToString() );
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
