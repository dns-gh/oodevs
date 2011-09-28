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
#include "Tools.h"
#include "UrbanModel.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Styles.h"
#include "clients_gui/TerrainObjectProxy.h"
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
    : EntityImplementation< Inhabitant_ABC >( controller, message.id().id(), QString( message.name().c_str() ) )
    , controller_      ( controller )
    , type_            ( type )
    , male_            ( ToInt( type_.GetMalePercentage() ) )
    , female_          ( ToInt( type_.GetFemalePercentage() ) )
    , children_        ( ToInt( type_.GetChildrenPercentage() ) )
    , dotationResolver_( dotationResolver )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.id().id() );
    Polygon2f polygon;
    for( int i = 0; i < message.objects_size(); ++i )
    {
        int id = message.objects( i ).id();
        gui::TerrainObjectProxy& proxy = model.GetObject( id );
        livingUrbanObject_[ id ] = &proxy;
        if( const UrbanPositions_ABC* positions = proxy.Retrieve< UrbanPositions_ABC >() )
        {
            polygon.Add( positions->Barycenter() );
            BOOST_FOREACH( const Polygon2f::T_Vertices::value_type& point, positions->Vertices() )
                boundingBox_.Incorporate( point );
        }
    }
    position_ = polygon.Barycenter();
    CreateDictionary();
    RegisterSelf( *this );
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
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller_ );
    Attach( dictionary );
    const Inhabitant& self = *this;
    const Entity_ABC& selfEntity = static_cast< const Entity_ABC& >( *this );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Info/Identifier" ), self.id_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Info/Name" ), self.name_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Male" ), self.male_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Female" ), self.female_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "M\\F\\C Repartition/Children" ), self.children_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Health" ), self.healthSatisfaction_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Safety" ), self.safetySatisfaction_ );
    dictionary.Register( selfEntity, tools::translate( "Inhabitant", "Satisfactions/Lodging" ), self.lodgingSatisfaction_ );
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
            const DotationType* dotation = dotationResolver_.Find( resource.resource().id() );
            if( !dotation )
                continue;
            resourceSatisfactions_[ dotation ] = ToInt( resource.value() );
            const QString key = tools::translate( "Inhabitant", "Satisfactions/Resource/" ) + dotation->GetName().c_str();
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
        CIT_UrbanObjectVector it = livingUrbanObject_.find( occupation.object().id() );
        if( it != livingUrbanObject_.end() )
            it->second->UpdateHumans( name_.ascii(), occupation );
    }
    if( msg.has_motivation() )
        motivation_ = msg.motivation();
    controller_.Update( *static_cast< Entity_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-05
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const Point2f& /*where*/, const Viewport_ABC& /*viewport*/, const GlTools_ABC& tools ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
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
    for( CIT_MotivationSatisfactions it = motivationSatisfactions_.begin(); it != motivationSatisfactions_.end(); ++it )
        displayer.Display( tools::translate( "Inhabitant", "%1 satisfaction:" ).arg( it->first.c_str() ), it->second );
    for( CIT_ResourceSatisfactions it = resourceSatisfactions_.begin(); it != resourceSatisfactions_.end(); ++it )
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
// Name: Inhabitant::IsAggregated
// Created: LGY 2011-03-07
// -----------------------------------------------------------------------------
bool Inhabitant::IsAggregated() const
{
    return false;
}
