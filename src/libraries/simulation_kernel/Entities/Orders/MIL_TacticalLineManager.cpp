//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_TacticalLineManager.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: MIL_TacticalLineManager.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_TacticalLineManager.h"
#include "TER_LimitData.h"
#include "MT_Tools/MT_ScipioException.h"

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLineManager::MIL_TacticalLineManager()
    : limitsData_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLineManager::~MIL_TacticalLineManager()
{
    for( CIT_LimitDataMap it = limitsData_.begin(); it != limitsData_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLimitData
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
const TER_LimitData& MIL_TacticalLineManager::CreateLimitData( const T_PointVector& points )
{
    TER_LimitData*& pData = limitsData_[ points ];
    if( !pData )
        pData = new TER_LimitData( points );
    return *pData;
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::DestroyLimitData
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::DestroyLimitData( const TER_LimitData& data )
{
    if( limitsData_.erase( data.GetPoints() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    delete &data;
}
