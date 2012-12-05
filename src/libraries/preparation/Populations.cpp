// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Populations.h"
#include "Population.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Populations constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
Populations::Populations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Populations destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
Populations::~Populations()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Populations::AddPopulation
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Populations::AddPopulation( Population& population )
{
    Register( population.GetId(), population );
}

// -----------------------------------------------------------------------------
// Name: Populations::SerializeAttributes
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Populations::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "populations" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "population" );
        it->second->Interface().Apply( &Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
}
