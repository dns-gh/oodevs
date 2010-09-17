// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_Tools_pch.h"
#include "MT_IdentifierManager.h"
#include <algorithm>

//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager constructor
// Created: NLD 2002-05-15
// Modified: RPD 2008-12-02
//-----------------------------------------------------------------------------
MT_IdentifierManager::MT_IdentifierManager()
    : nUpperBoundID_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager destructor
// Created: NLD 2002-05-15
// Modified: RPD 2008-12-02
//-----------------------------------------------------------------------------
MT_IdentifierManager::~MT_IdentifierManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager::LockIdentifier
// Created: NLD 2002-05-15
// Modified: RPD 2008-12-02
//-----------------------------------------------------------------------------
bool MT_IdentifierManager::LockIdentifier( unsigned long nWantedID )
{
    if( nWantedID < nUpperBoundID_ )
    {
        for ( IT_IDList itID = lockedIDList_.begin() ; itID != lockedIDList_.end() ; ++itID )
        {
            if ( *itID == nWantedID )
                return false;
            else if ( *itID > nWantedID )
            {
                lockedIDList_.insert( itID, nWantedID );
                return true;
            }
        }
        lockedIDList_.push_back( nWantedID );
    }
    else
    {
        lockedIDList_.push_back( nWantedID );
        nUpperBoundID_ = nWantedID + 1;
    }
    return true;
}


//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager::GetFreeIdentifier
// Created: NLD 2002-05-15
// Modified: RPD 2008-12-02
//-----------------------------------------------------------------------------
unsigned long MT_IdentifierManager::GetFreeIdentifier()
{
    if ( lockedIDList_.size() == nUpperBoundID_ )
    {
        lockedIDList_.push_back( nUpperBoundID_ );
        return nUpperBoundID_++;
    }
    else
    {
        unsigned long nIndex( 0 );
        for ( IT_IDList itID = lockedIDList_.begin(); itID != lockedIDList_.end() ; ++itID)
        {
            if ( *itID != nIndex )
            {
                lockedIDList_.insert( itID, nIndex );
                return nIndex;
            }
            ++nIndex;
        }
    }
    lockedIDList_.push_back( nUpperBoundID_ );
    return nUpperBoundID_++;
}

//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager::ReleaseIdentifier
// Created: NLD 2002-05-15
// Modified: RPD 2008-12-02
//-----------------------------------------------------------------------------
bool MT_IdentifierManager::ReleaseIdentifier( unsigned long nID )
{
    IT_IDList itID = std::find( lockedIDList_.begin(), lockedIDList_.end(), nID );
    if ( itID == lockedIDList_.end() )
        return false;
    lockedIDList_.erase( itID );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MT_IdentifierManager::ClearIdentifierManager
// Created: RPD 2008-12-02
//-----------------------------------------------------------------------------
void MT_IdentifierManager::ClearIdentifierManager()
{
    nUpperBoundID_ = 0;
    lockedIDList_.clear();
}
