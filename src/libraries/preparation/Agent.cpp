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
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const Automat_ABC& automat, const AgentType& type, Controller& controller, IdManager& idManager, bool commandPost )
    : controller_( controller )
    , id_( idManager.GetNextId() )
    , name_( type.GetName() )
    , automat_( automat )
    , type_( &type )
    , commandPost_( commandPost )
{
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xistream& xis, const Automat_ABC& automat, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : controller_( controller )
    , automat_( automat )
    , commandPost_( false )
{
    std::string name, type;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name )
        >> attribute( "type", type )
        >> optional() >> attribute( "command-post", commandPost_ );
    name_ = name.c_str();
    type_ = &agentTypes.Resolver< AgentType, QString >::Get( type.c_str() );
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    controller_.Delete( *(Agent_ABC*)this );
    DestroyExtensions();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
    if( commandPost_ )
        return QString( "[PC] - %1 [%2]" ).arg( name_ ).arg( id_ );
    return QString( "%1 [%2]" ).arg( name_ ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
const Automat_ABC& Agent::GetAutomat() const
{
    return automat_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
        type_->Draw( where, viewport, tools );
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
    controller_.Update( *(Agent_ABC*)this );
    controller_.Update( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary()
{
    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    dictionary.Register( tools::translate( "Agent", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Agent", "Info/Nom" ), name_ );
    dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), automat_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_->GetName().ascii() )
        << attribute( "name", name_.ascii() )
        << attribute( "command-post", commandPost_ );
}
