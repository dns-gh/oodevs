// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "tools/IdManager.h"

using namespace tools;

// -----------------------------------------------------------------------------
// Name: IdManager constructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
IdManager::IdManager( bool bKeepIds /*= false */)
    : max_( 1 )
    , bKeepIds_( bKeepIds )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdManager destructor
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
IdManager::~IdManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdManager::GetNextId
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
unsigned long IdManager::GetNextId( bool bSkipKeepId /*= false*/ )
{
    if( bKeepIds_ && !bSkipKeepId )
        ids_.insert( max_ );
    return max_++;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Reset
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void IdManager::Reset()
{
    max_ = 1;
}

// -----------------------------------------------------------------------------
// Name: IdManager::Lock
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void IdManager::Lock( unsigned long id, bool bSkipKeepId /*= false*/ )
{
    if( bKeepIds_ && !bSkipKeepId )
        ids_.insert( id );
    if( id >= max_ )
        max_ = id + 1;
}

// -----------------------------------------------------------------------------
// Name: IdManager::SetKeepIds
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void IdManager::SetKeepIds( bool bKeepIds )
{
    bKeepIds_ = bKeepIds;
    if( !bKeepIds )
        ids_.clear();
}

// -----------------------------------------------------------------------------
// Name: IdManager::HasBeenKept
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
bool IdManager::HasBeenKept( unsigned long id ) const
{
    if( !bKeepIds_ || ids_.empty() )
        return false;
    return ( ids_.find( id ) != ids_.end() );
}
