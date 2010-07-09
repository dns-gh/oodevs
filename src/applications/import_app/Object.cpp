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
// Name: Object destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object::~Object()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object::Object( const Object& rhs )
: name_( rhs.name_ )
, type_( rhs.type_ )
, positions_( rhs.positions_ )
, objectId_( rhs.objectId_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::operator=
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Object& Object::operator=( const Object& rhs)
{    
    name_ = rhs.name_;
    type_ = rhs.type_;
    positions_ = rhs.positions_;
    objectId_ = rhs.objectId_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Object::Read
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Object::Read( xml::xistream& xis, const std::string& id, Mapping& mapping )
{
    objectId_ = mapping.AddId( id );
    std::string metaId;
    xis >> xml::content( "ns2:name", name_ )
        >> xml::start( "ns2:meta-entity-ref" )
            >> xml::content( "ns2:id", metaId )
        >> xml::end()
        >> xml::start( "ns5:shape" )
            >> xml::start( "ns4:relative-geometry" )
                >> xml::optional()
                >> xml::start( "content" )
                    >> xml::start( "points" )
                        >> xml::start( "content" )
                            >> xml::list( "ns4:gdc", *this, &Object::ReadPositions )
                        >> xml::end()
                    >> xml::end()
                >> xml::end()
            >> xml::end();
    if( positions_.empty() )
    {
        xis >> xml::start( "ns4:center" )
            >> xml::start( "ns4:gdc" );
        Position center( xis );
        positions_.push_back( center );
    }
    type_ = mapping[ metaId ];
}

// -----------------------------------------------------------------------------
// Name: Object::Write
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Object::Write( xml::xostream& xos ) const
{
    xos << xml::start( "object" )
            << xml::attribute( "id", objectId_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", type_ )
            << xml::start( "shape" )
                << xml::attribute( "type", positions_.size() == 1 ? "point" : "polygone" )
                << xml::start( "points" );
    for( std::vector< Position >::const_iterator it = positions_.begin(); it != positions_.end(); ++it )
        it->Write( xos );
    xos         << xml::end()
            << xml::end()
            << xml::start( "attributes" )
                << xml::start( "obstacle" )
                    << xml::attribute( "activated", true )
                    << xml::attribute( "type", "preliminary" )
                << xml::end()
                << xml::start( "activity-time" )
                    << xml::attribute( "value", 0 )
                << xml::end()
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Object::ReadPositions
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Object::ReadPositions( xml::xistream& xis )
{
    positions_.push_back( Position( xis ) );
}
