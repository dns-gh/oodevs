// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Populations.h"
#include <xeumeuleu/xml.hpp>

Populations::Populations( kernel::Controllers& controllers )
    : tools::TrackingResolver< Population, kernel::Population_ABC >( controllers )
{
    // NOTHING
}

Populations::~Populations()
{
    DeleteAll();
}

void Populations::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "populations" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "population" );
        it->second->GetInterfaces().Apply( &Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
}
