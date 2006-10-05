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
#include "KnowledgeGroupHierarchy.h"
#include "CommunicationHierarchies.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AutomatType& type, Controller& controller, IdManager& idManager )
    : controller_( controller )
    , id_( idManager.GetNextId() )
    , name_( type.GetName() )
    , automat_( 0 )
    , automatType_( &type )
    , type_( automatType_->GetTypePC() )
    , aggregated_( false )
{
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const Agent_ABC& parent, const AgentType& type, Controller& controller, IdManager& idManager )
    : controller_( controller )
    , id_( idManager.GetNextId() )
    , name_( type.GetName() )
    , automat_( &parent )
    , automatType_( 0 )
    , type_( &type )
    , aggregated_( false )
{
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xistream& xis, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : controller_( controller )
    , automat_( 0 )
    , aggregated_( false )
{
    std::string name, type;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name )
        >> attribute( "type", type );
    name_ = name.c_str();
    automatType_ = &agentTypes.Resolver< AutomatType, QString >::Get( type.c_str() );
    type_ = automatType_->GetTypePC();
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary();
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xistream& xis, const Agent_ABC& parent, Controller& controller, IdManager& idManager, const AgentTypes& agentTypes )
    : controller_( controller )
    , automat_( &parent )
    , automatType_( 0 )
    , aggregated_( false )
{
    std::string name, type;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name )
        >> attribute( "type", type );
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
    DestroyExtensions();
    controller_.Delete( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
QString Agent::GetName() const
{
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
// Name: Agent::GetKnowledgeGroup
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC& Agent::GetKnowledgeGroup() const
{
    const kernel::CommunicationHierarchies* root = Retrieve< kernel::CommunicationHierarchies >();
    while( root && root->GetSuperior() )
    {
        if( const KnowledgeGroupHierarchy* group = root->GetEntity().Retrieve< KnowledgeGroupHierarchy >() )
            if( group->GetKnowledgeGroup() )
                return const_cast< KnowledgeGroup_ABC& >( *group->GetKnowledgeGroup() );
        root = root->GetSuperior()->Retrieve< kernel::CommunicationHierarchies >();
    }
    throw std::runtime_error( "I have no knowledge group" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSuperior
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent_ABC* Agent::GetSuperior() const
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
    {
        if( automatType_ && ! aggregated_ )
            automatType_->Draw( where, viewport, tools );
        if( type_ && ! aggregated_ )
            type_->Draw( where, viewport, tools );
        else if( type_ && aggregated_ )
            type_->DrawAggregated( where, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Agent::Aggregate( const bool& bDenis )
{
    aggregated_ = bDenis;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomatType
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
const AutomatType* Agent::GetAutomatType() const
{
    return automatType_;
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
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary()
{
    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    dictionary.Register( tools::translate( "Agent", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Agent", "Info/Nom" ), name_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", automatType_ ? automatType_->GetName().ascii() : type_->GetName().ascii() )
        << attribute( "name", name_.ascii() );
}
