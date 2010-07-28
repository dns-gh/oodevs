// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Object.h"
#include "Mapping.h"

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object::Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, const std::string& id, Mapping& mapping )
{
    objectId_ = mapping.AddId( id );
    std::string metaId;
    xis >> xml::content( "name", name_ )
        >> xml::start( "meta-entity-ref" )
            >> xml::content( "id", metaId )
        >> xml::end
        >> xml::start( "shape" )
            >> xml::start( "relative-geometry" )
                >> xml::optional
                >> xml::start( "content" )
                    >> xml::start( "points" )
                        >> xml::start( "content" )
                            >> xml::list( "gdc", *this, &Object::ReadPositions )
                        >> xml::end
                    >> xml::end
                >> xml::end
            >> xml::end;
    if( positions_.empty() )
    {
        xis >> xml::start( "center" )
                >> xml::start( "gdc" );
        positions_.push_back( Position( xis ) );
    }
    type_ = mapping[ metaId ];
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object::~Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Object& object )
{
    xos << xml::start( "object" )
            << xml::attribute( "id", object.objectId_ )
            << xml::attribute( "name", object.name_ )
            << xml::attribute( "type", object.type_ )
            << xml::start( "shape" )
                << xml::attribute( "type", object.positions_.size() == 1 ? "point" : "polygone" )
                << xml::start( "points" );
    for( std::vector< Position >::const_iterator it = object.positions_.begin(); it != object.positions_.end(); ++it )
        xml::content( "point", *it );
    return xos << xml::end
            << xml::end
            << xml::start( "attributes" )
                << xml::start( "obstacle" )
                    << xml::attribute( "activated", true )
                    << xml::attribute( "type", "preliminary" )
                << xml::end
                << xml::start( "activity-time" )
                    << xml::attribute( "value", 0 )
                << xml::end
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Object::ReadPositions
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Object::ReadPositions( xml::xistream& xis )
{
    positions_.push_back( Position( xis ) );
}
