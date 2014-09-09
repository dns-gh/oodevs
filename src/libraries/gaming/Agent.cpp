// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Agent.h"
#include "Attributes.h"
#include "Diplomacies.h"
#include "equipments.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message,
              Controller& controller,
              const tools::Resolver_ABC< AgentType >& resolver,
              const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : EntityImplementation< Agent_ABC >( controller, message.unit().id(), QString( message.name().c_str() ), canBeRenamedFunctor )
    , type_       ( resolver.Get( message.type().id() ) )
    , controller_( controller )
    , initialized_( false )
    , weight_( 0 )
    , speed_( 0 )
    , direction_( 0 )
    , sensorsDirection_( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( type_.GetLocalizedName().c_str() ).arg( message.unit().id() );

    level_ = ( message.has_level() ) ? "levels/" + ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( message.level() ) ) : type_.GetLevelSymbol();
    symbol_ = ( message.has_app6symbol() ) ? "symbols/" + message.app6symbol() : type_.GetSymbol();
    moveSymbol_ = type_.GetMoveSymbol();
    staticSymbol_ = type_.GetStaticSymbol();

    AddExtension( *this );
    CreateDictionary();
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    Destroy();
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !initialized_ )
    {
        initialized_ = true;
        const Entity_ABC& team = Get< CommunicationHierarchies >().GetTop();
        const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
        kernel::App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
        kernel::App6Symbol::SetKarma( moveSymbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
        kernel::App6Symbol::SetKarma( staticSymbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
    }
    if( viewport.IsHotpointVisible() && !IsAggregated() )
    {
        bool isMoving = ( Get< Attributes >().nCurrentPosture_ <= eUnitPosture_PostureArret );
        float depth = isMoving? type_.GetLength( !Get< Attributes >().bAmbianceSafety_ ) : type_.GetDepth();
        float width = isMoving? 0 : type_.GetWidth();
        unsigned int direction = isMoving ? direction_ : sensorsDirection_;
        tools.DrawUnitSymbol( symbol_, moveSymbol_, staticSymbol_, level_, isMoving, where, -1.f, direction, width, depth );
        bool app6 = isMoving ? moveSymbol_.empty() : staticSymbol_.empty();
        if( app6 )
            tools.DrawApp6SymbolFixedSize( level_, where, -1.f, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::ShouldDisplayStaticSymbol
// Created: LDC 2013-04-12
// -----------------------------------------------------------------------------
bool Agent::ShouldDisplayStaticSymbol() const
{
    return !staticSymbol_.empty();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSensorsDirection
// Created: LDC 2013-04-16
// -----------------------------------------------------------------------------
unsigned int Agent::GetSensorsDirection() const
{
    return sensorsDirection_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetMoveDirection
// Created: LDC 2013-04-23
// -----------------------------------------------------------------------------
unsigned int Agent::GetMoveDirection() const
{
    return direction_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSymbolWidth
// Created: LDC 2013-04-23
// -----------------------------------------------------------------------------
float Agent::GetSymbolWidth() const
{
    bool isMoving = ( Get< Attributes >().nCurrentPosture_ <= eUnitPosture_PostureArret );
    return isMoving? 0.f : type_.GetWidth();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSymbolDepth
// Created: LDC 2013-04-23
// -----------------------------------------------------------------------------
float Agent::GetSymbolDepth() const
{
    bool isMoving = ( Get< Attributes >().nCurrentPosture_ <= eUnitPosture_PostureArret );
    return isMoving? type_.GetLength( !Get< Attributes >().bAmbianceSafety_ ) : type_.GetDepth();
}

// -----------------------------------------------------------------------------
// Name: Agent::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Agent::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::DisplayInTooltip
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void Agent::DisplayInTooltip( Displayer_ABC& displayer ) const
{
     displayer.Item( "" ).Start( Styles::bold )
                 .Add( (Agent_ABC*)this )
              .End();
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Type" ), type_, true );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Weight" ), weight_, true );
}

// -----------------------------------------------------------------------------
// Name: Agent::NotifyUpdated
// Created: NPT 2012-11-16
// -----------------------------------------------------------------------------
void Agent::NotifyUpdated( const Equipments_ABC& equipments )
{
    if( Retrieve< Equipments_ABC >() == &equipments )
    {
        weight_ = equipments.GetTotalWeight();
        controller_.Update( gui::DictionaryUpdated( *( Entity_ABC* )this, tools::translate( "Agent", "Info" ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_speed()  )
        speed_ = float( message.speed() );
    if( message.has_direction() )
        direction_ = message.direction().heading();
    if( message.has_sensors_direction() )
        sensorsDirection_ = message.sensors_direction().heading();
    if( message.has_name() )
        SetName( QString::fromStdString( message.name() ) );
    Touch();
}
