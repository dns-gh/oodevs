// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_IDManager.h"

unsigned long MIL_IDManager::last_ = 1;
bool MIL_IDManager::bKeepIds_ = false;
std::set< unsigned long > MIL_IDManager::ids_;

// -----------------------------------------------------------------------------
// Name: MIL_IDManager constructor
// Created: SBO 2009-12-14
// -----------------------------------------------------------------------------
MIL_IDManager::MIL_IDManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::Destructor
// Created: MGD 2009-08-13
// -----------------------------------------------------------------------------
MIL_IDManager::~MIL_IDManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::SetKeepIdsMode
// Created: MMC 2013-05-27
// -----------------------------------------------------------------------------
void MIL_IDManager::SetKeepIdsMode( bool bKeepIds, unsigned long lastIdBeforeKeepIds /*= 1*/ )
{
    if( bKeepIds )
    {
        if( lastIdBeforeKeepIds > 1 )
            last_ = lastIdBeforeKeepIds;
    }
    else
        ids_.clear();
    bKeepIds_ = bKeepIds;
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::GetId
// Created: MMC 2013-05-27
// -----------------------------------------------------------------------------
unsigned long MIL_IDManager::GetId( unsigned long wishedId /*= 0*/, bool bForceId /*= false*/ )
{
    if( bForceId && wishedId != 0 )
    {
        if( bKeepIds_ )
            ids_.insert( wishedId );
        if( wishedId >= last_ )
            last_ = wishedId + 1;
        return wishedId;
    }
    if( wishedId == 0 || ( !bKeepIds_ && wishedId < last_ ) || ( bKeepIds_ && !ids_.insert( wishedId ).second ) )
    {
        if( bKeepIds_ )
            ids_.insert( last_ );
        return last_++;
    }
    if( wishedId >= last_ )
        last_ = wishedId + 1;
    return wishedId;
}
