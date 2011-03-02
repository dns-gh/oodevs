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

using namespace kernel;
// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( const kernel::PopulationType& type, int number, kernel::Controller& controller, IdManager& idManager )
    : kernel::EntityImplementation< kernel::Population_ABC >( controller, idManager.GetNextId(), "" )
    , type_( type )
    , healthy_( number )
    , wounded_( 0 )
    , dead_( 0 )
    , contaminated_( 0 )
    , armedIndividuals_( unsigned int( type.GetArmedIndividuals() * 100 ), Units::percentage )
    , male_ ( unsigned int( type.GetMale() * 100 ), Units::percentage )
    , female_ ( unsigned int( type.GetFemale() * 100 ), Units::percentage )
    , children_ ( unsigned int( type.GetChildren() * 100 ), Units::percentage )
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
    , healthy_( xis.attribute< int >( "healthy" ) )
    , wounded_( xis.attribute< int >( "wounded" ) )
    , dead_( xis.attribute< int >( "dead" ) )
    , contaminated_( xis.attribute< int >( "contaminated" ) )
    , armedIndividuals_ ( unsigned int( xis.attribute< float >( "armed-individuals", type_.GetArmedIndividuals() ) * 100 ) , Units::percentage )
    , male_ ( unsigned int( xis.attribute< float >( "male", type_.GetMale() ) * 100 ), Units::percentage )
    , female_ ( unsigned int( xis.attribute< float >( "female", type_.GetFemale() ) * 100 ), Units::percentage )
    , children_ ( unsigned int( xis.attribute< float >( "children", type_.GetChildren() ) * 100 ), Units::percentage )
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
    return healthy_ + wounded_ + dead_ + contaminated_ ;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    return dead_;
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Mood" ), attitude_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Info/Armed-Individuals" ), armedIndividuals_ );

    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Humans/Healthy" ), healthy_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Humans/Contaminated" ), contaminated_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Humans/Wounded" ), wounded_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "Humans/Dead" ), dead_ );

    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "M\\F\\C ratio/Males" ), male_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "M\\F\\C ratio/Females" ), female_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Crowd", "M\\F\\C ratio/Children" ), children_ );
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
        << xml::attribute( "healthy", healthy_ )
        << xml::attribute( "wounded", wounded_ )
        << xml::attribute( "dead", dead_ )
        << xml::attribute( "contaminated", contaminated_ )
        << xml::attribute( "armed-individuals", armedIndividuals_.value_ / 100 )
        << xml::attribute( "male", male_.value_ / 100 )
        << xml::attribute( "female", female_.value_/ 100 )
        << xml::attribute( "children", children_.value_/ 100 )
        << xml::attribute( "attitude", attitude_.ToXml() );
}
