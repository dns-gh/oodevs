//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Fuseau.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: MIL_Fuseau.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_TacticalLineManager.h"

#include "TER_LimitData.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLineManager::MIL_TacticalLineManager()
    : limitsData_()
{
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
    int nOut = limitsData_.erase( data.GetPoints() );
    assert( nOut == 1 );
    delete &data;
}

        
