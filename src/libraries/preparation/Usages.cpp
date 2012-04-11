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
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( xml::xistream& xis, std::auto_ptr< kernel::Usages_ABC > pUsages )
    : pUsages_( pUsages )
{
    xis >> xml::optional
        >> xml::start( "physical" )
            >> xml::optional
                >> xml::start( "usages" )
                    >> xml::list( "usage", *this, &Usages::ReadUsages )
                >> xml::end
        >> xml::end;
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
// Name: Usages::ReadUsages
// Created: LGY 2012-04-11
// -----------------------------------------------------------------------------
void Usages::ReadUsages( xml::xistream& xis )
{
    Add( xis.attribute< std::string >( "type" ), static_cast< unsigned int >( ( xis.attribute< float >( "proportion" ) + 0.001 ) * 100u ) );
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
