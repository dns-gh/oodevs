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
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( const kernel::PopulationType& type, int number, kernel::Controller& controller, IdManager& idManager )
    : kernel::EntityImplementation< kernel::Population_ABC >( controller, idManager.GetNextId(), "" )
    , type_( type )
    , livingHumans_( number )
    , attitude_( ePopulationAttitude_Calme )
{
    RegisterSelf( *this );
    name_ = ( type.GetName().c_str() + QString( " [%1]" ) ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const tools::Resolver_ABC< kernel::PopulationType, std::string >& types )
    : kernel::EntityImplementation< kernel::Population_ABC >( controller, xis.attribute< int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_( types.Get( xis.attribute< std::string >( "type" ) ) )
    , livingHumans_( xis.attribute< int >( "humans" ) )
    , attitude_( xis.attribute< std::string >( "attitude" ).c_str() )
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
    return 0;
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Type" ), constSelf.type_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Humans" ), (int&)livingHumans_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Mood" ), attitude_ );
}

// -----------------------------------------------------------------------------
// Name: Population::SerializeAttributes
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Population::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "type", type_.GetName() )
        << xml::attribute( "humans", long( livingHumans_ ) )
        << xml::attribute( "attitude", attitude_.ToXml() );
}
