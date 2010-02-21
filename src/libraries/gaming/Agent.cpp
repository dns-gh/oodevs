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
#include "clients_kernel/AgentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/Styles.h"
#include "Diplomacies.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const MsgsSimToClient::MsgUnitCreation& message, Controller& controller,  const tools::Resolver_ABC< AgentType >& resolver )

    : EntityImplementation< Agent_ABC >( controller, message.oid(), QString( message.nom().c_str() ) )
    , type_( resolver.Get( message.type_pion() ) )
    , isPc_( message.pc() != 0 )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( type_.GetName().c_str() ).arg( message.oid() );
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
    if( viewport.IsHotpointVisible() )
    {
        if( symbol_.empty() )
            InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, -1.f );
        tools.DrawApp6Symbol( type_.GetLevelSymbol(), where, -1.f );
        if( isPc_ )
            tools.DrawApp6Symbol( type_.GetHQSymbol(), where, -1.f );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Agent::InitializeSymbol() const
{
    symbol_ = type_.GetSymbol();
    const Entity_ABC& team = Get< CommunicationHierarchies >().GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    kernel::App6Symbol::SetKarma( symbol_, diplo ? diplo->GetKarma() : Karma::unknown_ );
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
// Name: Agent::IsCommandPost
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return isPc_;
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
// Name: Agent::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Agent::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetPosition
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const geometry::Point2d& Agent::GetPosition() const
{
     throw std::exception( __FUNCTION__ " not implemented" ); 
}
