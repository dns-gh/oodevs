//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/IDManager.cpp $
// $Author: Ape $
// $Modtime: 15/03/05 11:01 $
// $Revision: 2 $
// $Workfile: IDManager.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "IDManager.h"

//-----------------------------------------------------------------------------
// Name: IDManager constructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
IDManager::IDManager( uint nMOSClassID, uint nStartInstanceID )
    : nMOSClassID_     ( nMOSClassID )
    , nStartInstanceID_( nStartInstanceID )
{
    
}


//-----------------------------------------------------------------------------
// Name: IDManager destructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
IDManager::~IDManager()
{
    
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: IDManager::LockIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
bool IDManager::LockIdentifier( uint nID )
{
    nID &= 0x3FFFFF;
    assert( nID >= nStartInstanceID_ );

    nID -= nStartInstanceID_;

    return idManager_.LockIdentifier( nID );
}

//-----------------------------------------------------------------------------
// Name: IDManager::GetFreeIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
uint IDManager::GetFreeIdentifier()
{
    uint nID = idManager_.GetFreeIdentifier();

    nID += nStartInstanceID_;
    nID |= ( nMOSClassID_ << 22 );
    return nID;
}

//-----------------------------------------------------------------------------
// Name: IDManager::ReleaseIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void IDManager::ReleaseIdentifier( uint nID )
{
    nID &= 0x3FFFFF;
    assert( nID >= nStartInstanceID_ );

    nID -= nStartInstanceID_;

    idManager_.ReleaseIdentifier( nID );   
}
