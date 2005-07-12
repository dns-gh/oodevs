//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_MOSIDManager.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:16 $
// $Revision: 3 $
// $Workfile: MIL_MOSIDManager.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_MOSIDManager.h"

uint MIL_MOSIDManager::nSimStartID_ = 0x200000; // 2 097 152 = 2^22 / 2
     
//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager constructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
MIL_MOSIDManager::MIL_MOSIDManager()
    : nMOSClassID_       ( 0 )
    , nNextSimInstanceID_( 0 )
{
    
}


//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager destructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
MIL_MOSIDManager::~MIL_MOSIDManager()
{
    
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::ConvertSimIDToMosID
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
uint MIL_MOSIDManager::ConvertSimIDToMosID( uint nID )
{
    if( nID & ~0x3FFFFF )
    {
        if( (nID >> 22) != nMOSClassID_ )
            throw MT_ScipioException( "MIL_MOSIDManager::ConvertIDSimToMos", __FILE__, __LINE__, MT_FormatString( "Invalid class ID (%d) for ID '%d'", nMOSClassID_, nID ) );
        return nID;
    }

    nID |= nMOSClassID_ << 22;
    return nID;
}

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::LockMosID
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
bool MIL_MOSIDManager::LockMosID( uint nID )
{
    if( !IsMosIDValid( nID ) )
        return false;
    nID &= 0x3FFFFF;
    return mosIDManager_.LockIdentifier( nID );
}

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::ReleaseMosID
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
bool MIL_MOSIDManager::ReleaseMosID( uint nID )
{
    if( !IsMosIDValid( nID ) )
        return false;
    nID &= 0x3FFFFF;
    mosIDManager_.ReleaseIdentifier( nID );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::GetFreeSimID
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
uint MIL_MOSIDManager::GetFreeSimID()
{
//    uint nID = simIDManager_.GetFreeIdentifier();
    uint nID = nNextSimInstanceID_++;

    nID += nSimStartID_;
    nID |= ( nMOSClassID_ << 22 );
    return nID;
}


//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::LockSimID
// Created: JVT 03-07-31
//-----------------------------------------------------------------------------
bool MIL_MOSIDManager::LockSimID( uint nID )
{
    assert( IsSimIDValid( nID ) );
    nID &= 0x3FFFFF;
    nID -= nSimStartID_;
//    simIDManager_.LockIdentifier( nID );
    if( nID < nNextSimInstanceID_ )
        return false;
    nNextSimInstanceID_ = nID + 1;
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::ReleaseSimID
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void MIL_MOSIDManager::ReleaseSimID( uint nID )
{
    assert( IsSimIDValid( nID ) );
    nID &= 0x3FFFFF;
    nID -= nSimStartID_;
    assert( nID < nNextSimInstanceID_ );
//    simIDManager_.ReleaseIdentifier( nID );
}

