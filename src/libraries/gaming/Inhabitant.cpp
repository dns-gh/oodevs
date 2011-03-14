// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Inhabitant.h"
#include "Tools.h"
#include "UrbanModel.h"
#include "InhabitantAffinities.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Styles.h"
#include "clients_gui/TerrainObjectProxy.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const sword::PopulationCreation& message, Controllers& controllers, const UrbanModel& model, const tools::Resolver_ABC< InhabitantType >& typeResolver,
                        const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::StaticModel& staticModel )
    : EntityImplementation< Inhabitant_ABC >( controllers.controller_, message.id().id(), QString( message.name().c_str() ) )
    , controllers_     ( controllers )
    , type_            ( typeResolver.Get( message.type().id() ) )
    , dotationResolver_( dotationResolver )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.id().id() );
    if( message.has_extension() )
    {
        DictionaryExtensions* extensions = new DictionaryExtensions( "orbat-attributes", staticModel.extensionTypes_ );
        for( int i = 0; i < message.extension().entries_size(); ++i )
        {
            extensions->SetValue( message.extension().entries( i ).name(), message.extension().entries( i ).value() );
            extensions_[ message.extension().entries( i ).name() ] = message.extension().entries( i ).value();
        }
        Attach( *extensions );
    }
    for( int i = 0; i < message.objects_size(); ++i )
    {
        int id = message.objects( i ).id();
        livingUrbanObject_[ id ] = &model.GetObject( id );
    }
    CreateDictionary( controllers.controller_ );
    RegisterSelf( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    controllers_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Inhabitant& self = *this;
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Info/Identifier" ), self.id_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Info/Name" ), self.name_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Satisfaction/Health" ), self.healthSatisfaction_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Satisfaction/Safety" ), self.safetySatisfaction_ );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Satisfaction/Lodging" ), self.lodgingSatisfaction_ );
    BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
    {
        QString info = tools::translate( "Inhabitant", "Details/" ) + extension.first.c_str();
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), info, extension.second );
    }
}

namespace
{
    unsigned int ToInt( float value )
    {
        return static_cast< unsigned int >( 100 * ( value + 0.005f ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DoUpdate
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DoUpdate( const sword::PopulationUpdate& msg )
{
    if( msg.has_healthy() )
        healthy_ = msg.healthy();
    if( msg.has_dead() )
        dead_ = msg.dead();
    if( msg.has_wounded() )
        wounded_ = msg.wounded();
    if( msg.has_satisfaction() )
    {
        if( msg.satisfaction().has_health() )
            healthSatisfaction_ = ToInt( msg.satisfaction().health() );
        if( msg.satisfaction().has_safety() )
            safetySatisfaction_ = ToInt( msg.satisfaction().safety() );
        if( msg.satisfaction().has_lodging() )
            lodgingSatisfaction_ = ToInt( msg.satisfaction().lodging() );
        for( int i = 0; i < msg.satisfaction().motivations_size(); ++i )
        {
            const sword::PopulationUpdate_MotivationSatisfaction& motivation = msg.satisfaction().motivations( i );
            motivationSatisfactions_[ motivation.motivation() ] = ToInt( motivation.percentage() );
            const QString key = tools::translate( "Inhabitant", "Satisfaction/Usage/" ) + motivation.motivation().c_str();
            PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
            if( !dictionary.HasKey( key ) )
            {
                CIT_MotivationSatisfactions it = motivationSatisfactions_.find( motivation.motivation() );
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), key, it->second );
            }
        }
        for( int i = 0; i < msg.satisfaction().resources_size(); ++i )
        {
            const sword::PopulationUpdate_ResourceSatisfaction& resource = msg.satisfaction().resources( i );
            const kernel::DotationType* dotation = dotationResolver_.Find( resource.resource().id() );
            if( !dotation )
                continue;
            resourceSatisfactions_[ dotation ] = ToInt( resource.value() );
            const QString key = tools::translate( "Inhabitant", "Satisfaction/Resource/" ) + dotation->GetName().c_str();
            PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
            if( !dictionary.HasKey( key ) )
            {
                const T_ResourceSatisfactions::const_iterator it = resourceSatisfactions_.find( dotation );
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), key, it->second );
            }
        }
    }
    for( int i = 0; i < msg.occupations_size(); ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& occupation = msg.occupations( i );
        unsigned int id = occupation.object().id();
        CIT_UrbanObjectVector it = livingUrbanObject_.find( id );
        if( it != livingUrbanObject_.end() )
        {
            it->second->UpdateHumans( name_.ascii(), occupation.number(), occupation.alerted(), occupation.confined() );
            T_Human& mutableHuman = humans_[ id ];
            mutableHuman.number_ = occupation.number();
            mutableHuman.alerted_ = occupation.alerted();
            mutableHuman.confined_ = occupation.confined();
            const T_Human& human = mutableHuman;
            PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
            const QString keyHuman = tools::translate( "Inhabitant", "Living Area/" ) + it->second->GetName().ascii() + "/";
            const QString keyNumber = keyHuman + tools::translate( "Inhabitant", "Resident" );
            if( !dictionary.HasKey( keyNumber ) )
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyNumber, human.number_ );
            const QString keyAlerted = keyHuman + tools::translate( "Inhabitant", "Alerted" );
            if( !dictionary.HasKey( keyAlerted ) )
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyAlerted, human.alerted_ );
            const QString keyConfined = keyHuman + tools::translate( "Inhabitant", "Confined" );
            if( !dictionary.HasKey( keyConfined ) )
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), keyConfined, human.confined_ );
        }
    }
    controllers_.controller_.Update( *static_cast< Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const Point2f& /*where*/, const Viewport_ABC& /*viewport*/, const GlTools_ABC& tools ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const Polygon2f* footprint = it->second->GetFootprint();
        if( footprint )
            tools.DrawConvexPolygon( footprint->Vertices() );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetPosition
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Point2f Inhabitant::GetPosition( bool ) const
{
    Polygon2f poly;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        poly.Add( it->second->Barycenter() );
    return poly.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetHeight
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
float Inhabitant::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsAt
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::IsAt( const Point2f& /*pos*/, float /*precision*/, float /*adaptiveFactor*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsIn
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::IsIn( const Rectangle2f& /*rectangle*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetBoundingBox
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Rectangle2f Inhabitant::GetBoundingBox() const
{
    Rectangle2f box;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const Polygon2f* polygon = it->second->GetFootprint();
        if( polygon )
            BOOST_FOREACH( const Polygon2f::T_Vertices::value_type& point, polygon->Vertices() )
                box.Incorporate( point );
    }
    return box;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Accept( LocationVisitor_ABC& /*visitor*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInTooltip
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( static_cast< const Inhabitant_ABC* >( this ) ).End();
    displayer.Display( tools::translate( "Inhabitant", "Alive:" ), healthy_ );
    displayer.Display( tools::translate( "Inhabitant", "Wounded:" ), wounded_ );
    displayer.Display( tools::translate( "Inhabitant", "Dead:" ), dead_ );
    displayer.Display( tools::translate( "Inhabitant", "Health satisfaction:" ), healthSatisfaction_ );
    displayer.Display( tools::translate( "Inhabitant", "Safety satisfaction:" ), safetySatisfaction_ );
    displayer.Display( tools::translate( "Inhabitant", "Lodging satisfaction:" ), lodgingSatisfaction_ );
    for( CIT_MotivationSatisfactions it = motivationSatisfactions_.begin(); it != motivationSatisfactions_.end(); ++it )
        displayer.Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( it->first.c_str() ), it->second );
    for( CIT_ResourceSatisfactions it = resourceSatisfactions_.begin(); it != resourceSatisfactions_.end(); ++it )
        displayer.Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( it->first->GetName().c_str() ), it->second );
    Get< InhabitantAffinities >().Display( &displayer );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInSummary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInSummary( Displayer_ABC& displayer ) const
{
    const_cast< Inhabitant* >( this )->displayers_.insert( &displayer );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::NotifyUpdated
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::NotifyUpdated( const Simulation::sEndTick& /*tick*/ )
{
    if( !displayers_.empty() )
    {
        for( std::set< Displayer_ABC* >::const_iterator it = displayers_.begin(); it != displayers_.end(); ++it )
        {
            ( *it )->Display( tools::translate( "Inhabitant", "Alive:" ), healthy_ )
                    .Display( tools::translate( "Inhabitant", "Wounded:" ), wounded_ )
                    .Display( tools::translate( "Inhabitant", "Dead:" ), dead_ )
                    .Display( tools::translate( "Inhabitant", "Health satisfaction:" ), healthSatisfaction_ )
                    .Display( tools::translate( "Inhabitant", "Safety satisfaction:" ), safetySatisfaction_ )
                    .Display( tools::translate( "Inhabitant", "Lodging satisfaction:" ), lodgingSatisfaction_ );
            for( CIT_MotivationSatisfactions satisfaction = motivationSatisfactions_.begin(); satisfaction != motivationSatisfactions_.end(); ++satisfaction )
                ( *it )->Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( satisfaction->first.c_str() ), satisfaction->second );
            for( CIT_ResourceSatisfactions resource = resourceSatisfactions_.begin(); resource != resourceSatisfactions_.end(); ++resource )
                ( *it )->Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( resource->first->GetName().c_str() ), resource->second );
            Get< InhabitantAffinities >().Display( *it );
        }
        displayers_.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::CanAggregate
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
bool Inhabitant::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool Inhabitant::IsAggregated() const
{
    return false;
}
