// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Inhabitant.h"
#include "IdManager.h"
#include "InhabitantPositions.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const kernel::InhabitantType& type, int number, const QString& name, kernel::Controller& controller, IdManager& idManager )
    : kernel::EntityImplementation< kernel::Inhabitant_ABC >( controller, idManager.GetNextId(), "" )
    , type_( type )
    , healthy_( number )
    , dead_( 0 )
    , wounded_( 0 )
{
    extensions_[ "nationality" ] = "";
    extensions_[ "ethnic" ] = "";
    extensions_[ "religion" ] = "";
    text_ = "";

    RegisterSelf( *this );
    name_ = name.isEmpty() ? tools::translate( "Population", "%1 [%2]" ).arg( type.GetName().c_str() ).arg( id_ ) : name;
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const tools::Resolver_ABC< kernel::InhabitantType, std::string >& types )
    : kernel::EntityImplementation< kernel::Inhabitant_ABC >( controller, xis.attribute< int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_( types.Get( xis.attribute< std::string >( "type" ) ) )
{
    xis >> xml::start( "composition" )
        >> xml::attribute( "healthy", healthy_ )
        >> xml::attribute( "wounded", wounded_ )
        >> xml::attribute( "dead", dead_ )
        >> xml::end;

    if( xis.has_child( "extensions" ) )
    {
        xis >> xml::start( "extensions" )
                >> xml::list( "entry", *this, &Inhabitant::ReadExtension )
            >> xml::end;
    }

    std::string text;
    xis >> xml::content( "information", text );
    text_ = text.c_str();
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::CreateDictionary
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary& dictionary = *new kernel::PropertiesDictionary( controller );
    Attach( dictionary );
    const Inhabitant& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Info/Type" ), constSelf.type_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Info/Info" ), text_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Human/Alive" ), (int&)healthy_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Human/Dead" ), (int&)dead_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", "Human/Wounded" ), (int&)wounded_ );

    for( IT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        std::string info = "Details/" + it->first;
        dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Inhabitant", info.c_str() ), it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SerializeAttributes
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "type", type_.GetName() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", long( healthy_ ) )
            << xml::attribute( "dead", long( dead_ ) )
            << xml::attribute( "wounded", long( wounded_ ) )
        << xml::end;
    if ( !extensions_.empty() )
    {
        xos << xml::start( "extensions" );
        for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        {
            xos << xml::start( "entry" )
                    << xml::attribute( "key", it->first )
                    << xml::attribute( "value", it->second )
                << xml::end;
        }
         xos << xml::end;
    }
    std::string text = text_.ascii();
    xos << xml::content( "information", text_ ); 
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-11-26
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const kernel::GlTools_ABC& tools ) const
{
    const InhabitantPositions* pos = static_cast< const InhabitantPositions* >( Retrieve< kernel::Positions >() );
    pos->Draw( tools );
}


// -----------------------------------------------------------------------------
// Name: Inhabitant::ReadExtension
// Created: SLG 2010-11-26
// -----------------------------------------------------------------------------
void Inhabitant::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key" ) ] = QString( xis.attribute< std::string >( "value" ).c_str() );
}