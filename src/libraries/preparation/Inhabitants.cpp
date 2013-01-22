// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Inhabitants.h"
#include "Inhabitant.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Inhabitants constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitants::Inhabitants()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitants destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitants::~Inhabitants()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Inhabitants::AddInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitants::AddInhabitant( Inhabitant& inhabitant )
{
    Register( inhabitant.GetId(), inhabitant );
}

// -----------------------------------------------------------------------------
// Name: Inhabitants::SerializeAttributes
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitants::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "inhabitants" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "inhabitant" );
        it->second->GetInterfaces().Apply( &Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
}
