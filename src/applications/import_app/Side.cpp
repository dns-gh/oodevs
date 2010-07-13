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
Side::Side( const xml::xistream& xis, Mapping& mapping )
    : id_     ( mapping.AddId( xis.attribute< std::string >( "id" ) ) )
    , name_   ( xis.content< std::string >( "ns2:name" ) )
    , mapping_( &mapping )
{
    std::string knowledgeGroup = std::string( "Groupe de connaissance de " ) + name_;
    knowledgeGroupId_ = mapping_->AddId( knowledgeGroup );
    ReadObjects( xis );
    ReadTactical( xis );
}

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::Side()
    : id_              ( 0 )
    , knowledgeGroupId_( 0 )
    , mapping_         ( 0 )
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
            << xml::end
            << xml::start( "logistic" )
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Side::WriteObjects
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::WriteObjects( xml::xostream& xos ) const
{
    xos << xml::start( "objects" );
    for( std::map< std::string, Object >::const_iterator it = objects_.begin(); it != objects_.end(); ++it )
        it->second.Write( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Side::WriteTactical
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::WriteTactical( xml::xostream& xos ) const
{
    xos << xml::start( "tactical" );
    root_->Write( xos );
    xos << xml::end;
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
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Side::ReadObjects
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::ReadObjects( xml::xisubstream xis )
{
    xis >> xml::start( "ns5:data" )
            >> xml::list( "ns5:installations", *this, &Side::ReadObjectList )
            >> xml::list( "ns5:obstacles", *this, &Side::ReadObjectList )
            >> xml::list( "ns5:zones", *this, &Side::ReadObjectList );
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
    xis >> xml::attribute( "id", id );
    objects_[ id ] = Object( xis, id, *mapping_ );
}

// -----------------------------------------------------------------------------
// Name: Side::ReadTactical
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Side::ReadTactical( xml::xisubstream xis )
{
    root_.reset( new Entity( xis, *mapping_, knowledgeGroupId_ ) );
}
