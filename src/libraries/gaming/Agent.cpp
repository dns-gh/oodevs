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
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Styles.h"
#include "Diplomacies.h"
#include "Tools.h"
#include "ENT/ENT_Tr_Gen.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const sword::UnitCreation& message, Controller& controller, const tools::Resolver_ABC< AgentType >& resolver )
    : EntityImplementation< Agent_ABC >( controller, message.unit().id(), QString( message.name().c_str() ) )
    , type_       ( resolver.Get( message.type().id() ) )
    , initialized_( false )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( type_.GetName().c_str() ).arg( message.unit().id() );

    level_ = ( message.has_level() ) ? "levels/" + ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( message.level() ) ) : type_.GetLevelSymbol();
    symbol_ = ( message.has_app6symbol() ) ? "symbols/" + message.app6symbol() : type_.GetSymbol();

    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( !initialized_ )
    {
        initialized_ = true;
        const Entity_ABC& team = Get< CommunicationHierarchies >().GetTop();
        const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
        kernel::App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
    }
    if( viewport.IsHotpointVisible() )
    {
        tools.DrawApp6Symbol( symbol_, where, -1.f );
        tools.DrawApp6Symbol( level_, where, -1.f );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
//void Agent::InitializeSymbol() const
//{
//    const Entity_ABC& team = Get< CommunicationHierarchies >().GetTop();
//    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
//    kernel::App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
//}

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
void Agent::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Agent& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Name" ), self.name_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: LDC 2012-10-01
// -----------------------------------------------------------------------------
QString Agent::GetShortName() const
{
    return kernel::EntityImplementation< kernel::Agent_ABC >::GetName();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: LDC 2012-10-01
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
    std::string longName = gui::LongNameHelper::GetEntityLongName( *this );
    if( longName.empty() )
        return kernel::EntityImplementation< kernel::Agent_ABC >::GetName();
    return longName.c_str();
}
