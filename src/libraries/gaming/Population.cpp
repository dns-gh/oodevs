// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationPartPositionsProxy.h"
#include "Tools.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"
#include <boost/foreach.hpp>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const sword::CrowdCreation& message, Controllers& controllers, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< PopulationType >& typeResolver )
    : EntityImplementation< Population_ABC >( controllers.controller_, message.crowd().id(), QString( message.name().c_str() ) )
    , controllers_         ( controllers )
    , converter_           ( converter )
    , type_                ( typeResolver.Get( message.type().id() ) )
    , male_                ( static_cast< unsigned int >( 100 * message.repartition().male() + 0.5f ) )
    , female_              ( static_cast< unsigned int >( 100 * message.repartition().female() + 0.5f ) )
    , children_            ( static_cast< unsigned int >( 100 * message.repartition().children() + 0.5f ) )
    , nDomination_         ( 0, false )
    , criticalIntelligence_( "" )
    , armedIndividuals_    ( 0, false )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( type_.GetName().c_str() ).arg( message.crowd().id() );
    RegisterSelf( *this );
    CreateDictionary( controllers_.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    controllers_.Unregister( *this );
    tools::Resolver< PopulationFlow_ABC >::DeleteAll();
    tools::Resolver< PopulationConcentration_ABC >::DeleteAll();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Population::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetHealthyHumans() const
{
    unsigned int healthy = 0;
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            healthy += it.NextElement().GetHealthyHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            healthy += it.NextElement().GetHealthyHumans();
    }
    return healthy;
}

// -----------------------------------------------------------------------------
// Name: Population::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetWoundedHumans() const
{
    unsigned int wounded = 0;
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            wounded += it.NextElement().GetWoundedHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            wounded += it.NextElement().GetWoundedHumans();
    }
    return wounded;
}

// -----------------------------------------------------------------------------
// Name: Population::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetContaminatedHumans() const
{
    unsigned int contaminated = 0;
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            contaminated += it.NextElement().GetContaminatedHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            contaminated += it.NextElement().GetContaminatedHumans();
    }
    return contaminated;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    unsigned int dead = 0;
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            dead += it.NextElement().GetDeadHumans();
    }
    return dead;
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowUpdate& message )
{
    static_cast< PopulationFlow& >( tools::Resolver< PopulationFlow_ABC >::Get( message.flow().id() ) ).Update( message );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationUpdate& message )
{
    static_cast< PopulationConcentration& >( tools::Resolver< PopulationConcentration_ABC >::Get( message.concentration().id() ) ).Update( message );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowCreation& message )
{
    if( ! tools::Resolver< PopulationFlow_ABC >::Find( message.flow().id() ) )
    {
        PopulationFlow* entity = new PopulationFlow( message, converter_ );
        entity->Attach< Positions >( *new PopulationPartPositionsProxy( *entity ) );
        tools::Resolver< PopulationFlow_ABC >::Register( message.flow().id(), *entity );
        ComputeCenter();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationCreation& message )
{
    if( ! tools::Resolver< PopulationConcentration_ABC >::Find( message.concentration().id() ) )
    {
        PopulationConcentration* entity = new PopulationConcentration( message, converter_, type_.GetDensity() );
        entity->Attach< Positions >( *new PopulationPartPositionsProxy( *entity ) );
        tools::Resolver< PopulationConcentration_ABC >::Register( message.concentration().id(), *entity );
        ComputeCenter();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowDestruction& message )
{
    delete tools::Resolver< PopulationFlow_ABC >::Find( message.flow().id() );
    tools::Resolver< PopulationFlow_ABC >::Remove( message.flow().id() );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationDestruction& message )
{
    delete tools::Resolver< PopulationConcentration_ABC >::Find( message.concentration().id() );
    tools::Resolver< PopulationConcentration_ABC >::Remove( message.concentration().id() );
    ComputeCenter();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdUpdate& message )
{
    std::set< std::string > updated;

    UPDATE_PROPERTY( message, nDomination_, domination, "Info", updated );

    if( message.has_critical_intelligence() )
    {
        criticalIntelligence_ = message.critical_intelligence().c_str();
        updated.insert( "Info" );
    }

    if( message.has_armed_individuals() )
    {
        unsigned int armedIndividuals = armedIndividuals_;
        armedIndividuals_ = static_cast< unsigned int >( 100 * message.armed_individuals() + 0.5f );
        if( armedIndividuals_ != armedIndividuals )
            updated.insert( "Info" );
    }

    BOOST_FOREACH( const std::string& content, updated )
        controllers_.controller_.Update( kernel::DictionaryUpdated( *static_cast< Entity_ABC* >( this ), tools::translate( "Crowd", content.c_str() ) ) );

    controllers_.controller_.Update( *static_cast< Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Population::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void Population::Draw( const Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( boundingBox_ ) )
    {
        {
            tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
        {
            tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
    }
}

namespace
{
    template< typename Entity, typename ConcreteEntity >
    void IncorporateBoundingBox( const tools::Resolver< Entity >& resolver, Rectangle2f& boundingBox, Point2f& center )
    {
        tools::Iterator< const Entity& > it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const ConcreteEntity& concreteEntity = static_cast< const ConcreteEntity& >( it.NextElement() );
            const Rectangle2f bbox = concreteEntity.GetBoundingBox();
            boundingBox.Incorporate( bbox.TopRight() );
            boundingBox.Incorporate( bbox.BottomLeft() );
            if( center.IsZero() )
                center = concreteEntity.GetPosition( true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Population::ComputeCenter
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Population::ComputeCenter()
{
    boundingBox_ = Rectangle2f();
    center_ = Point2f();
    IncorporateBoundingBox< PopulationConcentration_ABC, PopulationConcentration >( *this, boundingBox_, center_ );
    IncorporateBoundingBox< PopulationFlow_ABC, PopulationFlow >( *this, boundingBox_, center_ );
}

// -----------------------------------------------------------------------------
// Name: Population::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Point2f Population::GetPosition( bool ) const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float Population::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Population::IsAt
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Population::IsAt( const geometry::Point2f& pos, float precision /* = 100.f*/, float adaptiveFactor ) const
{
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision, adaptiveFactor ) )
                return true;
        }
    }
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationFlow& concreteEntity = static_cast< const PopulationFlow& >( it.NextElement() );
            if( concreteEntity.IsAt( pos, precision, adaptiveFactor ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool Population::IsIn( const Rectangle2f& rectangle ) const
{
    {
        tools::Iterator< const PopulationConcentration_ABC& > it = tools::Resolver< PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationConcentration& concreteEntity = static_cast< const PopulationConcentration& >( it.NextElement() );
            if( concreteEntity.IsIn( rectangle ) )
                return true;
        }
    }
    {
        tools::Iterator< const PopulationFlow_ABC& > it = tools::Resolver< PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const PopulationFlow& concreteEntity = static_cast< const PopulationFlow& >( it.NextElement() );
            if( concreteEntity.IsIn( rectangle ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
Rectangle2f Population::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: Population::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void Population::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: Population::GetType
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
const PopulationType& Population::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetCriticalIntelligence
// Created: ABR 2012-07-05
// -----------------------------------------------------------------------------
kernel::CriticalIntelligence& Population::GetCriticalIntelligence()
{
    return criticalIntelligence_;
}

// -----------------------------------------------------------------------------
// Name: Population::CreateDictionary
// Created: JSR 2011-03-18
// -----------------------------------------------------------------------------
void Population::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Entity_ABC& selfEntity = static_cast< const Entity_ABC& >( *this );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Identifier" ), id_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Name" ), name_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Type" ), type_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Critical intelligence" ), criticalIntelligence_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Domination" ), nDomination_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "Info/Armed individuals" ), armedIndividuals_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "M\\F\\C Repartition/Male" ), male_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "M\\F\\C Repartition/Female" ), female_, true );
    dictionary.Register( selfEntity, tools::translate( "Crowd", "M\\F\\C Repartition/Children" ), children_, true );
}

// -----------------------------------------------------------------------------
// Name: Population::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Population::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( static_cast< Population_ABC* >( const_cast< Population* >( this ) ) ).End();
    displayer.Display( tools::translate( "Crowd", "Healthy:" ), GetHealthyHumans() );
    displayer.Display( tools::translate( "Crowd", "Wounded:" ), GetWoundedHumans() );
    displayer.Display( tools::translate( "Crowd", "Contaminated:" ), GetContaminatedHumans() );
    displayer.Display( tools::translate( "Crowd", "Dead:" ), GetDeadHumans() );
    displayer.Display( tools::translate( "Crowd", "Male:" ), male_ );
    displayer.Display( tools::translate( "Crowd", "Female:" ), female_ );
    displayer.Display( tools::translate( "Crowd", "Children:" ), children_ );
    displayer.Display( tools::translate( "Crowd", "Domination:" ), nDomination_ );
    displayer.Display( tools::translate( "Crowd", "Critical intelligence:" ), criticalIntelligence_ );
    displayer.Display( tools::translate( "Crowd", "Armed individuals:" ), armedIndividuals_ );
}

// -----------------------------------------------------------------------------
// Name: Population::DisplayInSummary
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void Population::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Crowd", "Healthy:" ), GetHealthyHumans() )
             .Display( tools::translate( "Crowd", "Wounded:" ), GetWoundedHumans() )
             .Display( tools::translate( "Crowd", "Contaminated:" ), GetContaminatedHumans() )
             .Display( tools::translate( "Crowd", "Dead:" ), GetDeadHumans() );
}

// -----------------------------------------------------------------------------
// Name: Population::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool Population::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Population::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool Population::IsAggregated() const
{
    return false;
}
