//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_IDManager.cpp $
// $Author: Ape $
// $Modtime: 15/03/05 11:01 $
// $Revision: 2 $
// $Workfile: MOS_IDManager.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"

#include "MOS_IDManager.h"

#ifndef MOS_USE_INLINE
#   include "MOS_IDManager.inl"
#endif

//-----------------------------------------------------------------------------
// Name: MOS_IDManager constructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
MOS_IDManager::MOS_IDManager( uint nMOSClassID, uint nStartInstanceID )
    : nMOSClassID_     ( nMOSClassID )
    , nStartInstanceID_( nStartInstanceID )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_IDManager destructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
MOS_IDManager::~MOS_IDManager()
{
    
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_IDManager::LockIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
bool MOS_IDManager::LockIdentifier( uint nID )
{
    nID &= 0x3FFFFF;
    assert( nID >= nStartInstanceID_ );

    nID -= nStartInstanceID_;

    return idManager_.LockIdentifier( nID );
}

//-----------------------------------------------------------------------------
// Name: MOS_IDManager::GetFreeIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
uint MOS_IDManager::GetFreeIdentifier()
{
    uint nID = idManager_.GetFreeIdentifier();

    nID += nStartInstanceID_;
    nID |= ( nMOSClassID_ << 22 );
    return nID;
}

//-----------------------------------------------------------------------------
// Name: MOS_IDManager::ReleaseIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_IDManager::ReleaseIdentifier( uint nID )
{
    nID &= 0x3FFFFF;
    assert( nID >= nStartInstanceID_ );

    nID -= nStartInstanceID_;

    idManager_.ReleaseIdentifier( nID );   
}
