// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Objects.h"
#include <xeumeuleu/xml.hpp>

Objects::Objects( kernel::Controllers& controllers )
    : tools::TrackingResolver< const Object, kernel::Object_ABC >( controllers )
{
    // NOTHING
}

Objects::~Objects()
{
    DeleteAll();
}

void Objects::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "objects" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "object" );
        it->second->SerializeAttributes( xos );
        xos     << xml::start( "attributes" );
            it->second->GetInterfaces().Apply( &Serializable_ABC::SerializeObjectAttributes, xos );
        xos     << xml::end
            << xml::end;
    }
    xos << xml::end;
}
