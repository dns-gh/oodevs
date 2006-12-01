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
#include "Diplomacies.h"
#include "Tools.h"

using namespace kernel;

const QString Agent::typeName_ = "agent";

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, Controller& controller,  const Resolver_ABC< AgentType >& resolver )
    : EntityImplementation< Agent_ABC >( controller, message.oid_pion, message.nom )
    , type_( resolver.Get( message.type_pion ) )
    , isPc_( message.pc )
{
    if( name_.isEmpty() )
        name_ = QString( "Agent %1" ).arg( message.oid_pion );
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
void Agent::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
    {
        if( symbol_.empty() )
            InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where );
        type_.Draw( where, viewport, tools, isPc_ );
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
    const Diplomacies* diplo = team.Retrieve< Diplomacies >();
    char karma = diplo ? diplo->GetKarma() : 'u';
    std::replace( symbol_.begin(), symbol_.end(), '*', karma );
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
// Name: Agent::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString Agent::GetTypeName() const
{
    return typeName_;
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
    displayer.Display( "", (Agent_ABC*)this );
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
