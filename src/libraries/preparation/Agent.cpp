// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Agent.h"
#include "IdManager.h"
#include "Tools.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AgentType& type, Controller& controller, IdManager& idManager, bool commandPost )
    : EntityImplementation< Agent_ABC >( controller, idManager.GetNextId(), type.GetName().c_str() )
    , type_( &type )
    , commandPost_( commandPost )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xml::xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : EntityImplementation< Agent_ABC >( controller, xml::attribute< unsigned long >( xis, "id" ), ReadName( xis ) )
    , commandPost_( false )
{
    std::string type;
    xis >> xml::attribute( "type", type )
        >> xml::optional() >> xml::attribute( "command-post", commandPost_ );
    type_ = &agentTypes.Resolver< AgentType, std::string >::Get( type );
    idManager.Lock( id_ );
    
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
//        if( symbol_.empty() ) // $$$$ SBO 2007-03-08: allow symbol update on team karma change... to avoid observation of team update
            InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where );
        tools.DrawApp6Symbol( type_->GetLevelSymbol(), where );
        if( commandPost_ )
            tools.DrawApp6Symbol( type_->GetHQSymbol(), where );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Agent::InitializeSymbol() const
{
    symbol_ = type_->GetSymbol();
    kernel::App6Symbol::SetKarma( symbol_, Get< CommunicationHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const AgentType& Agent::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsCommandPost
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return commandPost_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Agent::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Name" ), name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Command post" ), commandPost_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "type", type_->GetName() )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "command-post", commandPost_ );
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