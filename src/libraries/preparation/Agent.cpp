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
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AgentType& type, Controller& controller, IdManager& idManager, bool commandPost )
    : EntityImplementation< Agent_ABC >( controller, idManager.GetNextId(), type.GetName() )
    , type_( &type )
    , commandPost_( commandPost )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
    symbol_ = type_->GetSymbol();
    std::replace( symbol_.begin(), symbol_.end(), '*', 'f' );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : EntityImplementation< Agent_ABC >( controller, ReadId( xis ), ReadName( xis ) )
    , commandPost_( false )
{
    std::string type;
    xis >> attribute( "type", type )
        >> optional() >> attribute( "command-post", commandPost_ );
    type_ = &agentTypes.Resolver< AgentType, QString >::Get( type.c_str() );
    idManager.Lock( id_ );
    
    RegisterSelf( *this );
    CreateDictionary( controller );
    symbol_ = type_->GetSymbol();
    std::replace( symbol_.begin(), symbol_.end(), '*', 'f' );
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
// Name: Agent::ReadId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
unsigned long Agent::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Agent::ReadName
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
QString Agent::ReadName( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    return name.c_str();
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
        type_->Draw( where, viewport, tools, commandPost_ );
    }
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
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_->GetName().ascii() )
        << attribute( "name", name_.ascii() )
        << attribute( "command-post", commandPost_ );
}
