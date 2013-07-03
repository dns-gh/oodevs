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
    , type_            ( type )
    , healthy_         ( number )
    , wounded_         ( 0 )
    , dead_            ( 0 )
    , contaminated_    ( 0 )
    , armedIndividuals_( static_cast< unsigned int >( type.GetArmedIndividuals() * 100 ), Units::percentage )
    , repartition_     ( new PopulationRepartition() )
    , attitude_        ( ePopulationAttitude_Calme )
    , knowledgeGroupId_( 0 )
{
    repartition_->male_ = static_cast< unsigned int >( type.GetMale() * 100 );
    repartition_->female_ = static_cast< unsigned int >( type.GetFemale() * 100 );
    repartition_->children_ = static_cast< unsigned int >( type.GetChildren() * 100 );
    RegisterSelf( *this );
    name_ = ( type.GetName().c_str() + QString( " [%L1]" ) ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
Population::Population( xml::xistream& xis, const kernel::PopulationType& type, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Population_ABC >( controller, xis.attribute< int >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_            ( type )
    , armedIndividuals_( 0, Units::percentage )
    , repartition_     ( new PopulationRepartition() )
    , attitude_        ( xis.attribute< std::string >( "attitude" ).c_str() )
    , knowledgeGroupId_( 0 )
{
    float male = type_.GetMale();
    float female = type_.GetFemale();
    float children = type_.GetChildren();
    float armedIndividuals = type_.GetArmedIndividuals();
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
        >> xml::end
        >> xml::optional >> xml::start( "critical-intelligence" )
            >> xml::attribute( "content", criticalIntelligence )
        >> xml::end;
    repartition_->male_ = static_cast< unsigned int >( ( male + 0.005f ) * 100 );
    repartition_->female_ = static_cast< unsigned int >( ( female + 0.005f ) * 100 );
    repartition_->children_ = static_cast< unsigned int >( ( children + 0.005f ) * 100 );
    armedIndividuals_.value_ = static_cast< unsigned int >( ( armedIndividuals + 0.005f ) * 100 );
    criticalIntelligence_ = criticalIntelligence.c_str();
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
    delete repartition_;
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
const PopulationType& Population::GetType() const
{
    return type_;
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
// Name: Population::SetKnowledgeGroupForSerialization
// Created: JSR 2013-07-03
// -----------------------------------------------------------------------------
void Population::SetKnowledgeGroupForSerialization( unsigned long knowledgeGroupId )
{
    knowledgeGroupId_ = knowledgeGroupId;
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
    displayer.Display( tools::translate( "Crowd", "Type:" ), type_ );
    displayer.Display( tools::translate( "Crowd", "Healthy:" ), healthy_ );
    displayer.Display( tools::translate( "Crowd", "Contaminated:" ), contaminated_ );
    displayer.Display( tools::translate( "Crowd", "Wounded:" ), wounded_ );
    displayer.Display( tools::translate( "Crowd", "Dead:" ), dead_ );
}

// -----------------------------------------------------------------------------
// Name: Population::CreateDictionary
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void Population::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Population& constSelf = *this;
    const Entity_ABC& constEntity = *static_cast< const Entity_ABC* >( this );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Name" ), name_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Type" ), constSelf.type_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Mood" ), attitude_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Armed-Individuals" ), armedIndividuals_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Info/Critical Intelligence" ), criticalIntelligence_ );

    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Healthy" ), healthy_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Contaminated" ), contaminated_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Wounded" ), wounded_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Humans/Dead" ), dead_ );

    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Repartition" ), repartition_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Male" ), static_cast< const PopulationRepartition& >( *repartition_ ).male_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Female" ), static_cast< const PopulationRepartition& >( *repartition_ ).female_ );
    dictionary.Register( constEntity, tools::translate( "Crowd", "Male\\Female\\Children/Children" ), static_cast< const PopulationRepartition& >( *repartition_ ).children_ );
}

// -----------------------------------------------------------------------------
// Name: Population::SerializeAttributes
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void Population::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", static_cast< long >( id_ ) )
        << xml::attribute( "name", name_.toAscii().constData() )
        << xml::attribute( "type", type_.GetName() )
        << xml::attribute( "attitude", attitude_.ToXml() );
    if( knowledgeGroupId_ != 0 )
        xos << xml::attribute( "knowledge-group", knowledgeGroupId_ );
    xos << xml::start( "composition" )
            << xml::attribute( "healthy", healthy_ )
            << xml::attribute( "wounded", wounded_ )
            << xml::attribute( "dead", dead_ )
            << xml::attribute( "contaminated", contaminated_ )
        << xml::end;
    if( armedIndividuals_.value_ > 0 )
        xos << xml::start( "armed-individuals" )
                << xml::attribute( "value", armedIndividuals_.value_ / 100.f )
            << xml::end;
    if( !criticalIntelligence_.isEmpty() )
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_.toAscii().constData() )
        << xml::end;
    if( repartition_->male_ != 100 )
        xos << xml::start( "repartition" )
                << xml::attribute( "male", repartition_->male_ / 100.f )
                << xml::attribute( "female", repartition_->female_ / 100.f )
                << xml::attribute( "children", repartition_->children_ / 100.f )
            << xml::end;
}
