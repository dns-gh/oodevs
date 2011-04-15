// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Usages.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( const sword::UrbanAttributes& message, std::auto_ptr< kernel::Usages_ABC > pUsages )
    : pUsages_( pUsages )
{
    for( int i = 0; i <  message.usages_size(); ++i )
    {
        const sword::UrbanUsage& usage =  message.usages( i );
        Add( usage.role(), usage.percentage() );
    }
}

// -----------------------------------------------------------------------------
// Name: Usages destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::~Usages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Usages::Add
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Usages::Add( const std::string& usage, unsigned int proportion )
{
    return pUsages_->Add( usage, proportion );
}

// -----------------------------------------------------------------------------
// Name: Usages::Find
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
unsigned int Usages::Find( const std::string& usage ) const
{
    return pUsages_->Find( usage );
}
