// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#include "simulation_tester_pch.h"
#include "IDManager.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: IDManager constructor
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
IDManager::IDManager( unsigned int nMOSClassID, unsigned int nStartInstanceID )
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
bool IDManager::LockIdentifier( unsigned int nID )
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
unsigned int IDManager::GetFreeIdentifier()
{
    unsigned int nID = idManager_.GetFreeIdentifier();

    nID += nStartInstanceID_;
    nID |= ( nMOSClassID_ << 22 );
    return nID;
}

//-----------------------------------------------------------------------------
// Name: IDManager::ReleaseIdentifier
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void IDManager::ReleaseIdentifier( unsigned int nID )
{
    nID &= 0x3FFFFF;
    assert( nID >= nStartInstanceID_ );

    nID -= nStartInstanceID_;

    idManager_.ReleaseIdentifier( nID );
}
