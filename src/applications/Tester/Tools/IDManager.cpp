// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "IDManager.h"

using namespace TEST;

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
