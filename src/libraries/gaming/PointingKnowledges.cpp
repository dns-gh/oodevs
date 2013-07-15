// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PointingKnowledges.h"
#include "clients_kernel/Knowledge_ABC.h"

// -----------------------------------------------------------------------------
// Name: PointingKnowledges constructor
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
PointingKnowledges::PointingKnowledges()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges destructor
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
PointingKnowledges::~PointingKnowledges()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::Add
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
void PointingKnowledges::Add( const kernel::Knowledge_ABC& knowledge )
{
    knowledges_.insert( &knowledge );
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::Remove
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
void PointingKnowledges::Remove( const kernel::Knowledge_ABC& knowledge )
{
    knowledges_.erase( &knowledge );
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::IsKnown
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
bool PointingKnowledges::IsKnown( unsigned int id ) const
{
    for( auto it = knowledges_.begin(); it != knowledges_.end(); ++it )
        if ( ( *it )->GetEntityId() == id )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PointingKnowledges::GetKnowledgeFromUnitId
// Created: NPT 2013-07-15
// -----------------------------------------------------------------------------
const kernel::Knowledge_ABC* PointingKnowledges::GetKnowledgeFromUnitId( unsigned int id ) const
{
    for( auto it = knowledges_.begin(); it != knowledges_.end(); ++it )
        if ( ( *it )->GetEntityId() == id )
            return *it;
    return 0;
}

