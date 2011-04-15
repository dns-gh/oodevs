// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Usages.h"
#include <urban/MotivationsVisitor_ABC.h>
#include <urban/TerrainObject_ABC.h>
#include <map>

namespace
{
    class UsagesVisitor : public urban::MotivationsVisitor_ABC
    {
    public:
        explicit UsagesVisitor( std::map< std::string, float >& usages )
            : usages_( usages )
        {
            // NOTHING
        }
        virtual void Visit( const std::string& usage, float proportion )
        {
            usages_[ usage ] = proportion;
        }
        std::map< std::string, float >& usages_;
    };
}

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( const urban::TerrainObject_ABC& object, std::auto_ptr< kernel::Usages_ABC > pUsages )
    : pUsages_( pUsages )
{
    std::map< std::string, float > usages;
    UsagesVisitor visitor( usages );
    object.Accept( visitor );
    for( std::map< std::string, float >::const_iterator it = usages.begin(); it != usages.end(); ++it )
        Add( it->first, static_cast< unsigned int >( ( it->second + 0.001 ) * 100u ) );
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
    pUsages_->Add( usage, proportion );
}

// -----------------------------------------------------------------------------
// Name: Usages::Find
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
unsigned int Usages::Find( const std::string& usage ) const
{
    return pUsages_->Find( usage );
}
