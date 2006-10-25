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
#include "Tools.h"

using namespace kernel;

const QString Agent::typeName_ = "agent";

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, Controller& controller,  const Resolver_ABC< AgentType >& resolver )
    : EntityImplementation< Agent_ABC >( controller, message.oid_pion, QString( "%1 [%2]" ).arg( message.nom ).arg( message.oid_pion ) )
    , type_( resolver.Get( message.type_pion ) )
    , isPc_( message.pc )
{
    symbol_     = type_.GetSymbol();
    std::replace( symbol_.begin(), symbol_.end(), '*', 'f' ); // $$$$ AGE 2006-10-24: hard coded friend
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
        tools.DrawApp6Symbol( symbol_, where );
        type_.Draw( where, viewport, tools, isPc_ );
    }
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
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Agent& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Identifiant" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Nom" ), self.name_ );
}
