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
#include "TeamHierarchy.h"
#include "KnowledgeGroupHierarchy.h"
#include "CommunicationHierarchies.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Formation_ABC.h"
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
Agent::Agent( const Formation_ABC& parent, const AutomatType& type, Controller& controller, IdManager& idManager )
    : controller_( controller )
    , id_( idManager.GetNextId() )
    , name_( type.GetName() )
    , formation_( &parent )
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
    , formation_( 0 )
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
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    DeleteAll();
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
    if( automat_ )
        return automat_->GetKnowledgeGroup();
    const KnowledgeGroupHierarchy* group = Retrieve< KnowledgeGroupHierarchy >();
    if( group )
        return *const_cast< KnowledgeGroup_ABC* >( group->GetKnowledgeGroup() );
    throw std::runtime_error( "I have no knowledge group" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTeam
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
const Team_ABC& Agent::GetTeam() const
{
    if( automat_ )
        return automat_->GetTeam();
    const TeamHierarchy* team = formation_->Retrieve< TeamHierarchy >();
    if( team )
        return team->GetTeam();
    throw std::runtime_error( "I have no team" );
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
// Name: Agent::IsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool Agent::IsInTeam( const Team_ABC& team ) const
{
    return &GetTeam() == &team;
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
    dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), formation_ ? (Entity_ABC*)formation_ : (Entity_ABC*)automat_ );
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
