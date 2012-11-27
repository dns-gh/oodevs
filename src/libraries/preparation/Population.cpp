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
#include "tools/IdManager.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( const PopulationType& type, int number, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Population_ABC >( controller, idManager.GetNextId(), "" )
    , healthy_         ( number )
    , wounded_         ( 0 )
    , dead_            ( 0 )
    , contaminated_    ( 0 )
    , armedIndividuals_( static_cast< unsigned int >( type.GetArmedIndividuals() * 100 ), Units::percentage )
    , repartition_     ( new PopulationRepartition() )
    , attitude_        ( ePopulationAttitude_Calme )
{
    repartition_->male_ = static_cast< unsigned int >( type.GetMale() * 100 );
    repartition_->female_ = static_cast< unsigned int >( type.GetFemale() * 100 );
    repartition_->children_ = static_cast< unsigned int >( type.GetChildren() * 100 );
    RegisterSelf( *this );
    name_ = type.GetName().c_str();
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( xml::xistream& xis, const kernel::PopulationType& type, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Population_ABC >( controller, xis.attribute< int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , armedIndividuals_( 0, Units::percentage )
    , repartition_     ( new PopulationRepartition() )
    , attitude_        ( xis.attribute< std::string >( "attitude" ).c_str() )
{
    float male = type.GetMale();
    float female = type.GetFemale();
    float children = type.GetChildren();
    float armedIndividuals = type.GetArmedIndividuals();
    std::string criticalIntelligence;
    xis >> xml::start( "composition" )
            >> xml::attribute( "healthy", healthy_ )
            >> xml::attribute( "wounded", wounded_ )
            >> xml::attribute( "dead", dead_ )
            >> xml::attribute( "contaminated", contaminated_ )
        >> xml::end
        >> xml::optional >> xml::start( "repartition" )
            >> xml::attribute( "male", male )
            >> xml::attribute( "female", female )
            >> xml::attribute( "children", children )
        >> xml::end
        >> xml::optional >> xml::start( "armed-individuals" )
            >> xml::attribute( "value", armedIndividuals )
        >> xml::end;
    repartition_->male_ = static_cast< unsigned int >( ( male + 0.005f ) * 100 );
    repartition_->female_ = static_cast< unsigned int >( ( female + 0.005f ) * 100 );
    repartition_->children_ = static_cast< unsigned int >( ( children + 0.005f ) * 100 );
    armedIndividuals_.value_ = static_cast< unsigned int >( ( armedIndividuals + 0.005f ) * 100 );
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::~Population()
{
    delete repartition_;
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Population::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetHealthyHumans() const
{
    return healthy_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetWoundedHumans() const
{
    return wounded_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetContaminatedHumans() const
{
    return contaminated_;
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
// Name: Population::GetTotalLivingHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetTotalLivingHumans() const
{
    return healthy_ + wounded_ + contaminated_;
}

// -----------------------------------------------------------------------------
// Name: Population::DisplayInTooltip
// Created: JSR 2012-03-13
// -----------------------------------------------------------------------------
void Population::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    QString id = QString( "[%L1]" ).arg( GetId() );
    displayer.Item( "" ).Start( Styles::bold ).Add( static_cast< const Population_ABC* >( this ) ).AddToDisplay( id );
    displayer.End();
    displayer.Display( tools::translate( "Crowd", "Healthy:" ), healthy_ );
    displayer.Display( tools::translate( "Crowd", "Contaminated:" ), contaminated_ );
    displayer.Display( tools::translate( "Crowd", "Wounded:" ), wounded_ );
    displayer.Display( tools::translate( "Crowd", "Dead:" ), dead_ );
}

// -----------------------------------------------------------------------------
// Name: Population::CreateDictionary
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void Population::CreateDictionary()
{
    PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
    const Entity_ABC& constEntity = *static_cast< const Entity_ABC* >( this );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Mood" ), attitude_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Armed-Individuals" ), armedIndividuals_ );

    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Healthy" ), healthy_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Contaminated" ), contaminated_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Wounded" ), wounded_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Dead" ), dead_ );

    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Repartition" ), repartition_, false, kernel::ePopulationRepartition );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Male" ), repartition_->male_, true, kernel::ePopulationRepartition );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Female" ), repartition_->female_, true, kernel::ePopulationRepartition );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Children" ), repartition_->children_, true, kernel::ePopulationRepartition );
}

// -----------------------------------------------------------------------------
// Name: Population::SerializeAttributes
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Population::SerializeAttributes( xml::xostream& xos ) const
{
    kernel::EntityImplementation< kernel::Population_ABC >::SerializeAttributes( xos );
    xos << xml::attribute( "attitude", attitude_.ToXml() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", healthy_ )
            << xml::attribute( "wounded", wounded_ )
            << xml::attribute( "dead", dead_ )
            << xml::attribute( "contaminated", contaminated_ )
        << xml::end;
    if( armedIndividuals_.value_ > 0 )
        xos << xml::start( "armed-individuals" )
                << xml::attribute( "value", armedIndividuals_.value_ / 100.f )
            << xml::end;
    if( repartition_->male_ != 100 )
        xos << xml::start( "repartition" )
                << xml::attribute( "male", repartition_->male_ / 100.f )
                << xml::attribute( "female", repartition_->female_ / 100.f )
                << xml::attribute( "children", repartition_->children_ / 100.f )
            << xml::end;
}
