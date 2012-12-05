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
#include "Object.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Objects constructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
Objects::Objects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objects destructor
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
Objects::~Objects()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Objects::AddObject
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
void Objects::AddObject( const Object& object )
{
    Register( object.GetId(), object );
}

// -----------------------------------------------------------------------------
// Name: Objects::SerializeAttributes
// Created: JSR 2011-02-22
// -----------------------------------------------------------------------------
void Objects::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "objects" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "object" );
        it->second->SerializeAttributes( xos );
        it->second->SerializePositions( xos );
        xos     << xml::start( "attributes" );
            it->second->Interface().Apply( &Serializable_ABC::SerializeObjectAttributes, xos );
        xos     << xml::end
            << xml::end;
    }
    xos << xml::end;
}
