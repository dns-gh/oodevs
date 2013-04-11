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
#include "Diplomacies.h"
#include "equipments.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr_Gen.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message, Controller& controller, const tools::Resolver_ABC< AgentType >& resolver )
    : EntityImplementation< Agent_ABC >( controller, message.unit().id(), QString( message.name().c_str() ), true )
    , type_       ( resolver.Get( message.type().id() ) )
    , controller_( controller )
    , initialized_( false )
    , weight_( 0 )
    , speed_( 0 )
    , direction_( 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( type_.GetName().c_str() ).arg( message.unit().id() );

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
    if( viewport.IsHotpointVisible() )
    {
        float width = type_.GetWidth();
        float depth = type_.GetDepth();
        tools.DrawUnitSymbol( symbol_, moveSymbol_, staticSymbol_, speed_ != 0, where, -1.f, direction_, width, depth );
        tools.DrawApp6SymbolFixedSize( level_, where, -1.f, 0 );
    }
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
    PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Type" ), type_, true );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Weight" ), weight_, true );
}

// -----------------------------------------------------------------------------
// Name: Agent::NotifyUpdated
// Created: NPT 2012-11-16
// -----------------------------------------------------------------------------
void Agent::NotifyUpdated( const Equipments& equipments )
{
    if( Retrieve< Equipments >() == &equipments )
    {
        weight_ = equipments.GetTotalWeight();
        controller_.Update( DictionaryUpdated( *( Entity_ABC* )this, tools::translate( "Agent", "Info" ) ) );
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
}
