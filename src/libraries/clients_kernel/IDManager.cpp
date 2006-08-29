// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "IDManager.h"
#include <cassert>

using namespace kernel;

//-----------------------------------------------------------------------------
// Name: IDManager constructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
IDManager::IDManager( uint nMOSClassID, uint nStartInstanceID )
    : nMOSClassID_     ( nMOSClassID )
    , nStartInstanceID_( nStartInstanceID )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: IDManager destructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
IDManager::~IDManager()
{
    // NOTHING
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
