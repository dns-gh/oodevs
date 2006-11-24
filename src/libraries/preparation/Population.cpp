// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Population.h"
#include "IdManager.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

namespace
{
    template< typename T >
    T ReadAttribute( xml::xistream& xis, const std::string& name )
    {
        T value;
        xis >> attribute( name, value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( const kernel::PopulationType& type, kernel::Controller& controller, IdManager& idManager )
    : kernel::EntityImplementation< kernel::Population_ABC >( controller, idManager.GetNextId(), type.GetName() )
    , type_( type )
    , livingHumans_( 1000 ) // $$$$ SBO 2006-11-08: hard coded
    , attitude_( ePopulationAttitude_Calme )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const kernel::Resolver_ABC< kernel::PopulationType, QString >& types )
    : kernel::EntityImplementation< kernel::Population_ABC >( controller, ReadAttribute< int >( xis, "id" ), ReadAttribute< std::string >( xis, "name" ).c_str() )
    , type_( types.Get( ReadAttribute< std::string >( xis, "type" ).c_str() ) )
    , livingHumans_( ReadAttribute< int >( xis, "humans" ) )
    , attitude_( ReadAttribute< std::string >( xis, "attitude" ).c_str() )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::~Population()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
const kernel::PopulationType& Population::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
unsigned int Population::GetLivingHumans() const
{
    return livingHumans_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: Population::CreateDictionary
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void Population::CreateDictionary( kernel::Controller& controller )
{
    kernel::PropertiesDictionary& dictionary = *new kernel::PropertiesDictionary( controller );
    Attach( dictionary );
    const Population& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Population", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Population", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Population", "Info/Type" ), constSelf.type_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Population", "Info/Humans" ), (int&)livingHumans_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Population", "Info/Mood" ), attitude_ );
}

// -----------------------------------------------------------------------------
// Name: Population::SerializeAttributes
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Population::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() )
        << attribute( "type", type_.GetName().ascii() )
        << attribute( "humans", long( livingHumans_ ) )
        << attribute( "attitude", attitude_.ToString() );
}
