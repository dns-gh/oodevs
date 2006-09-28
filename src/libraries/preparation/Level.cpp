// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Level.h"

using namespace kernel;

unsigned int Level::idManager_ = 1;

// -----------------------------------------------------------------------------
// Name: Level constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
Level::Level( const QString& name, const HierarchyLevel_ABC* previous )
    : name_( name )
    , id_( idManager_++ )
    , previous_( previous )
    , next_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Level destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
Level::~Level()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Level::SetNext
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Level::SetNext( const HierarchyLevel_ABC& next )
{
    next_ = &next;
}

// -----------------------------------------------------------------------------
// Name: Level::GetId
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
unsigned int Level::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Level::GetName
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
QString Level::GetName() const
{
    return name_;
}
    
// -----------------------------------------------------------------------------
// Name: Level::GetPrevious
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* Level::GetPrevious() const
{
    return previous_;
}
    
// -----------------------------------------------------------------------------
// Name: Level::GetNext
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* Level::GetNext() const
{
    return next_;
}

// -----------------------------------------------------------------------------
// Name: Level::operator<
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
bool Level::operator<( const kernel::HierarchyLevel_ABC& rhs ) const
{
    for( const kernel::HierarchyLevel_ABC* current = &rhs; current; current = current->GetPrevious() )
        if( current == this )
            return false;
    return true;
}
