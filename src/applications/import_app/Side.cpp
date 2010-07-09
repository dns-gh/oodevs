// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Side.h"

#include "Mapping.h"

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::Side( unsigned int id, const std::string& name, Mapping& mapping )
: id_( id )
, name_( name )
, mapping_( &mapping )
{
    std::string knowledgeGroup = std::string( "Groupe de connaissance de " ) + name_;
    knowledgeGroupId_ = mapping_->AddId( knowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::Side()
: id_( 0 )
, knowledgeGroupId_( 0 )
, mapping_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Side destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::~Side()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::Side( const Side& rhs )
: id_( rhs.id_ )
, knowledgeGroupId_( rhs.knowledgeGroupId_ )
, name_( rhs.name_ )
, mapping_( rhs.mapping_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Side::operator=
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side& Side::operator=( const Side& rhs )
{
    id_ = rhs.id_;
    knowledgeGroupId_ = rhs.knowledgeGroupId_;
    name_ = rhs.name_;
    mapping_ = rhs.mapping_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Side::WriteSide
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::Write( xml::xostream& xos ) const
{
    xos << xml::start( "side" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "name", name_ )
            << xml::attribute( "type", "friend" );
    WriteObjects( xos );
    WriteTactical( xos );
    WriteCommunications( xos );
    xos    << xml::start( "populations" )
            << xml::end()
            << xml::start( "logistic" )
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Side::WriteObjects
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::WriteObjects( xml::xostream& xos ) const
{
    xos << xml::start( "objects" );
    for( std::map< std::string, Object >::const_iterator it = objects_.begin(); it != objects_.end(); ++it )
    {
        it->second.Write( xos );
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Side::WriteTactical
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::WriteTactical( xml::xostream& xos ) const
{
    xos << xml::start( "tactical" );
    root_->Write( xos, *mapping_ );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Side::WriteCommunications
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::WriteCommunications( xml::xostream& xos ) const
{
    xos << xml::start( "communication" )
            << xml::start( "knowledge-group" )
                << xml::attribute( "id", knowledgeGroupId_ )
                << xml::attribute( "name", std::string( "Groupe de connaissance de " ) + name_ )
                << xml::attribute( "type", "Standard" )
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Side::ReadObjects
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::ReadObjects( xml::xistream& xis )
{
    xis >> xml::start( "ns5:data" )
            >> xml::list( "ns5:installations", *this, &Side::ReadObjectList )
            >> xml::list( "ns5:obstacles", *this, &Side::ReadObjectList )
            >> xml::list( "ns5:zones", *this, &Side::ReadObjectList )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Side::ReadObjectList
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::ReadObjectList( xml::xistream& xis )
{
    xis >> xml::list( "ns5:content", *this, &Side::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: Side::ReadObject
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::ReadObject( xml::xistream& xis )
{
    std::string id;
    std::string name;
    std::string metaId;
    xis >> xml::attribute( "id", id );
    objects_[ id ].Read( xis, id, *mapping_ );
}

// -----------------------------------------------------------------------------
// Name: Side::ReadTactical
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Side::ReadTactical( xml::xistream& xis )
{
    xis >> xml::start( "ns5:order-of-battle" )
            >> xml::start( "ns5:content" ); // Formation of highest level;
    root_.reset( new Entity() );
    root_->Read( xis, *mapping_, knowledgeGroupId_ );
}

// -----------------------------------------------------------------------------
// Name: Side::Root
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Entity& Side::Root()
{
    return *root_;
}
