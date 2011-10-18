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
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const kernel::InhabitantType& type, int number, const QString& name, kernel::Controller& controller, IdManager& idManager )
    : kernel::EntityImplementation< kernel::Inhabitant_ABC >( controller, idManager.GetNextId(), "" )
    , type_      ( type )
    , healthNeed_( 0 )
    , text_      ( "" )
{
    healthy_ = number;
    unsigned int healthPeopleNumber = type_.GetHealthPeopleNumber();
    if( healthPeopleNumber )
        healthNeed_ = static_cast< float >( number ) / healthPeopleNumber;
    RegisterSelf( *this );
    name_ = name.isEmpty() ? tools::translate( "Population", "%1 [%2]" ).arg( type.GetName().c_str() ).arg( id_ ) : name;
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const tools::StringResolver< kernel::InhabitantType >& types )
    : kernel::EntityImplementation< kernel::Inhabitant_ABC >( controller, xis.attribute< int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_( types.Get( xis.attribute< std::string >( "type" ) ) )
{
    xis >> xml::start( "composition" )
            >> xml::attribute( "healthy", healthy_ )
            >> xml::attribute( "wounded", wounded_ )
            >> xml::attribute( "dead", dead_ )
        >> xml::end;

    xis >> xml::start( "health-need" )
            >> xml::attribute( "quantity", healthNeed_ )
        >> xml::end;

    std::string text;
    xis >> xml::start( "information" ) >> xml::optional >> text
        >> xml::end;

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
    const Entity_ABC& constEntity = *static_cast< const Entity_ABC* >( this );
    dictionary.Register( constEntity, tools::translate( "Population", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Info/Name" ), name_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Info/Type" ), constSelf.type_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Info/Info" ), text_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Human/Alive" ), healthy_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Human/Wounded" ), wounded_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Human/Dead" ), dead_ );
    dictionary.Register( constEntity, tools::translate( "Population", "Health/Infrastructures needed" ), healthNeed_ );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SerializeAttributes
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void Inhabitant::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "type", type_.GetName() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", healthy_ )
            << xml::attribute( "wounded", wounded_ )
            << xml::attribute( "dead", dead_ )
        << xml::end
        << xml::start( "health-need" )
            << xml::attribute( "quantity", healthNeed_ )
        << xml::end
        << xml::content( "information", text_ );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetType
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
const kernel::InhabitantType& Inhabitant::GetType() const
{
    return type_;
}
