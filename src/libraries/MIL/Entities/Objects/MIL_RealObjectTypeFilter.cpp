// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_RealObjectTypeFilter.cpp $
// $Author: Nld $
// $Modtime: 26/10/04 16:00 $
// $Revision: 1 $
// $Workfile: MIL_RealObjectTypeFilter.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RealObjectTypeFilter.h"
#include "MIL_RealObjectType.h"

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter constructor
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_RealObjectTypeFilter::MIL_RealObjectTypeFilter()
    : objectTypeStates_( MIL_RealObjectType::GetObjectTypes().size(), false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter constructor
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_RealObjectTypeFilter::MIL_RealObjectTypeFilter( DIA_Parameters& diaParameters, uint nFirstDiaParam )
    : objectTypeStates_( MIL_RealObjectType::GetObjectTypes().size(), false )
{
    uint nNbrParams = diaParameters.GetParameters().size();
    for( uint i = nFirstDiaParam; i < nNbrParams; ++i )
    {
        const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( diaParameters.GetParameter( i ).ToId() );
        assert( pObjectType );
        Set( *pObjectType );
    }    
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter destructor
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
MIL_RealObjectTypeFilter::~MIL_RealObjectTypeFilter()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter::Set
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
void MIL_RealObjectTypeFilter::Set()
{
    objectTypeStates_.insert( objectTypeStates_.begin(), objectTypeStates_.size(), true );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter::Set
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
void MIL_RealObjectTypeFilter::Set( const MIL_RealObjectType& objectType )
{
    assert( objectTypeStates_.size() > objectType.GetID() );
    objectTypeStates_[ objectType.GetID() ] = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter::Reset
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
void MIL_RealObjectTypeFilter::Reset( const MIL_RealObjectType& objectType )
{
    assert( objectTypeStates_.size() > objectType.GetID() );
    objectTypeStates_[ objectType.GetID() ] = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectTypeFilter::Test
// Created: NLD 2004-10-22
// -----------------------------------------------------------------------------
bool MIL_RealObjectTypeFilter::Test( const MIL_RealObjectType& objectType ) const
{
    assert( objectTypeStates_.size() > objectType.GetID() );
    return objectTypeStates_[ objectType.GetID() ];
}
