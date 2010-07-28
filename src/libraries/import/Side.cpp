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
Side::Side( xml::xisubstream xis, Mapping& mapping )
    : mapping_         ( &mapping )
    , id_              ( mapping.AddId( xis.attribute< std::string >( "id" ) ) )
    , name_            ( xis.content< std::string >( "name" ) )
    , knowledgeGroupId_( mapping.AddId( "Groupe de connaissance de " + name_ ) )
{
    xis >> xml::start( "data" )
            >> xml::list( "installations", *this, &Side::ReadObjects )
            >> xml::list( "obstacles", *this, &Side::ReadObjects )
            >> xml::list( "zones", *this, &Side::ReadObjects )
        >> xml::end
        >> xml::start( "order-of-battle" )
            >> xml::start( "content" ); // Formation of highest level
    root_.reset( new Entity( xis, mapping, knowledgeGroupId_ ) );
}

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Side::Side()
    : id_              ( 0 )
    , knowledgeGroupId_( 0 )
    , mapping_         ( 0 ) // $$$$ MCO : hmm !
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
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Side& side )
{
    xos << xml::start( "side" )
            << xml::attribute( "id", side.id_ )
            << xml::attribute( "name", side.name_ )
            << xml::attribute( "type", "friend" );
    xos << xml::start( "objects" );
    for( std::map< std::string, Object >::const_iterator it = side.objects_.begin(); it != side.objects_.end(); ++it )
        xos << it->second;
    return xos << xml::end
        << xml::content( "tactical", *side.root_ )
        << xml::start( "communication" )
            << xml::start( "knowledge-group" )
                << xml::attribute( "id", side.knowledgeGroupId_ )
                << xml::attribute( "name", std::string( "Groupe de connaissance de " ) + side.name_ )
                << xml::attribute( "type", "Standard" )
            << xml::end
        << xml::end
        << xml::start( "populations" )
            << xml::end
            << xml::start( "logistic" )
            << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Side::ReadObjects
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Side::ReadObjects( xml::xistream& xis )
{
    xis >> xml::list( "content", *this, &Side::ReadObject );
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
