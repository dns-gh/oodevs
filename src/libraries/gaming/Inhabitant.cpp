// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Inhabitant.h"
#include "UrbanModel.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/UrbanObject.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"
#include <boost/foreach.hpp>

using namespace geometry;
using namespace kernel;

namespace
{
    inline unsigned int ToInt( float value )
    {
        return static_cast< unsigned int >( 100 * ( value + 0.005f ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( const sword::PopulationCreation& message, Controller& controller, const UrbanModel& model, const kernel::InhabitantType& type,
                        const tools::Resolver_ABC< DotationType >& dotationResolver )
    : EntityImplementation< Inhabitant_ABC >( controller, message.id().id(), QString( message.name().c_str() ), true )
    , controller_      ( controller )
    , male_            ( ToInt( type.GetMalePercentage() ) )
    , female_          ( ToInt( type.GetFemalePercentage() ) )
    , children_        ( ToInt( type.GetChildrenPercentage() ) )
    , dotationResolver_( dotationResolver )
    , livingUrbanObjects_       ( 0 )
    , nominalCapacity_          ( 0 )
    , infrastructures_          ( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( type.GetName().c_str() ).arg( message.id().id() );
    Polygon2f polygon;
    for( int i = 0; i < message.objects_size(); ++i )
    {
        int id = message.objects( i ).id();
        kernel::UrbanObject_ABC& object = model.GetObject( id );
        livingUrbanObject_[ id ] = &object;
        if( const UrbanPositions_ABC* positions = object.Retrieve< UrbanPositions_ABC >() )
        {
            polygon.Add( positions->Barycenter() );
            BOOST_FOREACH( const Polygon2f::T_Vertices::value_type& point, positions->Vertices() )
                boundingBox_.Incorporate( point );
        }
    }
    position_ = polygon.Barycenter();
    CreateDictionary();
    AddExtension( *this );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    controller_.Unregister( *this );
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::CreateDictionary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    const Entity_ABC& selfEntity = static_cast< const Entity_ABC& >( *this );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Male" ), male_, true );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Female" ), female_, true );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Children" ), children_, true );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Health" ), healthSatisfaction_, true );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Safety" ), safetySatisfaction_, true );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Lodging" ), lodgingSatisfaction_, true );
    UpdateUrbanObjectsDictionnary();
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
            const QString key = tools::translate( "Inhabitant", "Satisfactions/Usage/" ) + motivation.motivation().c_str();
            gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
            if( !dictionary.HasKey( key ) )
            {
                auto it = motivationSatisfactions_.find( motivation.motivation() );
                dictionary.Register( *static_cast< const Entity_ABC* >( this ), key, it->second );
            }
        }
        for( int i = 0; i < msg.satisfaction().resources_size(); ++i )
        {
            const sword::PopulationUpdate_ResourceSatisfaction& resource = msg.satisfaction().resources( i );
            const DotationType* dotation = dotationResolver_.Find( resource.resource().id() );
            if( !dotation )
                continue;
            resourceSatisfactions_[ dotation ] = ToInt( resource.value() );
            const QString key = tools::translate( "Inhabitant", "Satisfactions/Resource/" ) + dotation->GetName().c_str();
            gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
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
        auto it = livingUrbanObject_.find( occupation.object().id() );
        if( it != livingUrbanObject_.end() )
            static_cast< gui::UrbanObject* >( it->second )->UpdateHumans( name_.toStdString(), occupation );
    }
    if( msg.has_motivation() )
        motivation_ = msg.motivation();
    UpdateUrbanObjectsDictionnary();
    controller_.Update( *static_cast< Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GlTools_ABC& tools ) const
{
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( const UrbanPositions_ABC* positions = it->second->Retrieve< UrbanPositions_ABC >() )
            tools.DrawPolygon( positions->Vertices() );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::GetPosition
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
Point2f Inhabitant::GetPosition( bool ) const
{
    return position_;
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
    return boundingBox_;
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
    displayer.Display( tools::translate( "Inhabitant", "Motivation:" ), motivation_ );
    displayer.Display( tools::translate( "Inhabitant", "Health satisfaction:" ), healthSatisfaction_ );
    displayer.Display( tools::translate( "Inhabitant", "Safety satisfaction:" ), safetySatisfaction_ );
    displayer.Display( tools::translate( "Inhabitant", "Lodging satisfaction:" ), lodgingSatisfaction_ );
    for( auto it = motivationSatisfactions_.begin(); it != motivationSatisfactions_.end(); ++it )
        displayer.Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( it->first.c_str() ), it->second );
    for( auto it = resourceSatisfactions_.begin(); it != resourceSatisfactions_.end(); ++it )
        displayer.Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( it->first->GetName().c_str() ), it->second );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DisplayInSummary
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Inhabitant", "Alive:" ), healthy_ )
             .Display( tools::translate( "Inhabitant", "Wounded:" ), wounded_ )
             .Display( tools::translate( "Inhabitant", "Dead:" ), dead_ )
             .Display( tools::translate( "Inhabitant", "Motivation:" ), motivation_ );
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
// Name: Inhabitant::UpdateUrbanObjectsDictionnary
// Created: MMC 2011-03-21
// -----------------------------------------------------------------------------
void Inhabitant::UpdateUrbanObjectsDictionnary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();

    accomodationCapacties_.clear();
    infrastructures_ = nominalCapacity_ = 0;
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        auto pObject = static_cast< const gui::UrbanObject* >( it->second );
        if( !pObject )
            continue;
        nominalCapacity_ += static_cast< unsigned int >( pObject->GetNominalCapacity() );
        if( auto infra = pObject->Retrieve< gui::Infrastructure_ABC >() )
            if( infra->GetType() != 0 )
                ++infrastructures_;
        const kernel::AccommodationTypes& accommodations = pObject->GetAccommodations();
        tools::Iterator< const kernel::AccommodationType& > itAcco = accommodations.CreateIterator();
        while( itAcco.HasMoreElements() )
        {
            const kernel::AccommodationType& accomodation = itAcco.NextElement();
            accomodationCapacties_[ QString::fromStdString( accomodation.GetRole() ) ] += static_cast< unsigned int >( pObject->GetNominalCapacity( accomodation.GetRole() ) );
        }
    }

    livingUrbanObjects_ = static_cast< unsigned int >( livingUrbanObject_.size() );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Living Area/Urban blocks number" ), static_cast< const unsigned int& >( livingUrbanObjects_ ) );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Living Area/Total capacity" ), static_cast< const unsigned int& >( nominalCapacity_ ) );
    dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Living Area/Non medical infrastructures" ), static_cast< const unsigned int& >( infrastructures_ ) );
    for( QMap< QString, unsigned int >::const_iterator it = accomodationCapacties_.constBegin(); it != accomodationCapacties_.constEnd(); ++it )
        dictionary.Register( *static_cast< const Entity_ABC* >( this ), tools::translate( "Inhabitant", "Living Area/Capacities/%1" ).arg( it.key() ), it.value() );
}