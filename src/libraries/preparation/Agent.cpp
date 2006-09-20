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
#include "Serializable_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

unsigned long Agent::idManager_ = 1; // $$$$ SBO 2006-09-05: 

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const KnowledgeGroup_ABC& gtia, const AutomatType& type, Controller& controller
            , const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , id_( idManager_++ )
    , name_( type.GetName() )
    , automatType_( &type )
    , type_( automatType_->GetTypePC() )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    RegisterSelf( *this );
    CreateDictionary();
    // $$$$ AGE 2006-09-20: 
    ChangeKnowledgeGroup( gtia.GetId() );
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const Agent_ABC& automat, const AgentType& type, Controller& controller
            , const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , id_( idManager_++ )
    , name_( type.GetName() )
    , automatType_( 0 )
    , type_( &type )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    RegisterSelf( *this );
    CreateDictionary();
    ChangeSuperior( automat.GetId() );
    controller_.Create( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    ChangeKnowledgeGroup( (kernel::KnowledgeGroup_ABC*)0 );
    ChangeSuperior( 0 );
     // $$$$ SBO 2006-09-04: 
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        static_cast< Agent* >( it->second )->superior_ = 0;
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
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( unsigned long id )
{
    ChangeKnowledgeGroup( gtiaResolver_.Find( id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( KnowledgeGroup_ABC* gtia )
{
    if( gtia_ )
        gtia_->RemoveAutomat( id_ );
    gtia_ = gtia;
    if( gtia_ )
        gtia_->AddAutomat( id_, *this );
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        static_cast< Agent* >( it->second )->gtia_ = gtia_;
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeSuperior
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeSuperior( unsigned long id )
{
    if( superior_ )
        superior_->RemoveChild( *this );
    superior_ = static_cast< Agent* >( agentResolver_.Find( id ) );
    if( superior_ )
        superior_->AddChild( *this );
    controller_.Update( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::AddChild( Agent_ABC& child )
{
    Resolver< Agent_ABC >::Register( child.GetId(), child );
    controller_.Update( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::RemoveChild( Agent_ABC& child )
{
    Resolver< Agent_ABC >::Remove( child.GetId() );
    controller_.Update( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetKnowledgeGroup
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC& Agent::GetKnowledgeGroup() const
{
    if( gtia_ )
        return *gtia_;
    if( superior_ )
        return superior_->GetKnowledgeGroup();
    throw std::runtime_error( "I have no knowledge group" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTeam
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
const Team_ABC& Agent::GetTeam() const
{
    return GetKnowledgeGroup().GetTeam();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSuperior
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent_ABC* Agent::GetSuperior() const
{
    return superior_;
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
    return GetKnowledgeGroup().IsInTeam( team );
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
    dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), superior_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::Serialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::Serialize( xml::xostream& xos ) const
{
    xos << start( automatType_ ? "Automate" : "Pion" )
            << attribute( "id", long( id_ ) )
            << attribute( "type", automatType_ ? automatType_->GetName().ascii() : type_->GetName().ascii() )
            << content( "Nom", name_ );
    Interface().Apply( &Serializable_ABC::Serialize, xos );
    xos         << start( "LiensHierarchiques" );
    if( automatType_ )
        xos << content( "Armee", GetTeam().GetName() )
            << content( "GroupeConnaissance", long( gtia_->GetId() ) );
    else
        xos << content( "Automate", long( superior_->GetId() ) );
    xos         << end()
        << end();
}
