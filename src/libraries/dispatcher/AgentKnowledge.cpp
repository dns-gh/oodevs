// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "KnowledgeGroup.h"
#include "Side.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( Model& model, const MsgsSimToClient::MsgUnitKnowledgeCreation& message )
    : SimpleEntity< kernel::AgentKnowledge_ABC >( message.oid() )
    , model_                ( model )
    , knowledgeGroup_       ( model.knowledgeGroups_.Get( message.oid_groupe_possesseur() ) )
    , agent_                ( model.agents_.Get( message.oid_unite_reelle() ) )
    , type_                 ( message.type_unite() )
    , nRelevance_           ( 0 )
    , nPerceptionLevel_     ( MsgsSimToClient::EnumUnitIdentificationLevel::signale )
    , nMaxPerceptionLevel_  ( MsgsSimToClient::EnumUnitIdentificationLevel::signale )
    , nOperationalState_    ( 0 )
    , bDead_                ( false )
    , nDirection_           ( 0 )
    , nSpeed_               ( 0 )
    , team_                 ( 0 )
    , bPC_                  ( false )
    , bSurrendered_         ( false )
    , bPrisoner_            ( false )
    , bRefugeeManaged_      ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    // NOTHING
}

#define UPDATE_ASN_ATTRIBUTE(MESSAGE, ASN, CPP ) \
    if( ##MESSAGE##.has_##ASN##() )              \
    {                                            \
        CPP = ##MESSAGE##.##ASN();               \
    }

#define SEND_ASN_ATTRIBUTE( MESSAGE, ASN, CPP )  \
    if ( ##MESSAGE##.has_##ASN##() )    \
    {                                   \
        ##MESSAGE##.set_##ASN ( CPP );  \
    }   

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message )
{
    UPDATE_ASN_ATTRIBUTE( message ,  pertinence               , nRelevance_            );
    UPDATE_ASN_ATTRIBUTE( message ,  identification_level     , nPerceptionLevel_      );
    UPDATE_ASN_ATTRIBUTE( message ,  max_identification_level , nMaxPerceptionLevel_   );
    UPDATE_ASN_ATTRIBUTE( message ,  etat_op                  , nOperationalState_     );
    UPDATE_ASN_ATTRIBUTE( message ,  mort                     , bDead_                 );

    if( message.has_position() )
        position_.Set( message.position().latitude(), message.position().longitude() );
   
    UPDATE_ASN_ATTRIBUTE( message ,  speed                    , nSpeed_                );

    if( message.has_direction() )
        nDirection_ = message.direction().heading();

    if( message.has_camp() )
        team_ = &model_.sides_.Get( message.camp() );

    UPDATE_ASN_ATTRIBUTE( message ,  nature_pc, bPC_ );

    if( message.has_perception_par_compagnie() )
    {
        automatePerceptions_.clear();
        for( int i = 0; i < message.perception_par_compagnie().elem_size(); ++i )
            automatePerceptions_.push_back( message.perception_par_compagnie().elem().Get(i) );
    }

    UPDATE_ASN_ATTRIBUTE( message ,  rendu                    , bSurrendered_          );
    UPDATE_ASN_ATTRIBUTE( message ,  prisonnier               , bPrisoner_             );
    UPDATE_ASN_ATTRIBUTE( message ,  refugie_pris_en_compte   , bRefugeeManaged_       );

    ApplyUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeCreation message;
    
    message().set_oid                       ( GetId() );
    message().set_oid_groupe_possesseur     ( knowledgeGroup_.GetId() );
    message().set_oid_unite_reelle          ( agent_.GetId() );
    message().mutable_type_unite()->set_type( type_.type() );

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeUpdate message;
    
    message().set_oid                   ( GetId() );
    message().set_oid_groupe_possesseur ( knowledgeGroup_.GetId() );

    SEND_ASN_ATTRIBUTE( message() ,  pertinence               , nRelevance_            );
    SEND_ASN_ATTRIBUTE( message() ,  identification_level     , nPerceptionLevel_      );
    SEND_ASN_ATTRIBUTE( message() ,  max_identification_level , nMaxPerceptionLevel_   );
    SEND_ASN_ATTRIBUTE( message() ,  etat_op                  , nOperationalState_     );
    SEND_ASN_ATTRIBUTE( message() ,  mort                     , bDead_                 );

    message().mutable_position()->set_latitude( position_.X() );
    message().mutable_position()->set_longitude( position_.Y() );
    message().mutable_direction()->set_heading( nDirection_ );
    SEND_ASN_ATTRIBUTE( message() ,  speed    , nSpeed_     );

    if( message().has_camp() )
    {
        assert( team_ );
        message().set_camp( team_->GetId() );
    }

    SEND_ASN_ATTRIBUTE( message() ,  nature_pc, bPC_ );

    for( unsigned int i = 0; i < automatePerceptions_.size(); ++i )
        *message().mutable_perception_par_compagnie()->add_elem() = automatePerceptions_[ i ];

    SEND_ASN_ATTRIBUTE( message() ,  rendu                    , bSurrendered_          );
    SEND_ASN_ATTRIBUTE( message() ,  prisonnier               , bPrisoner_             );
    SEND_ASN_ATTRIBUTE( message() ,  refugie_pris_en_compte   , bRefugeeManaged_       );

    message.Send( publisher );

    if( message().has_perception_par_compagnie() && message().perception_par_compagnie().elem_size() > 0 )
        message().mutable_perception_par_compagnie()->Clear();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void AgentKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeDestruction message;
    message().set_oid                  ( GetId() );
    message().set_oid_groupe_possesseur( knowledgeGroup_.GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void AgentKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetRecognizedEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AgentKnowledge::GetRecognizedEntity() const
{
    return &agent_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Agent_ABC* AgentKnowledge::GetEntity() const
{
    return &agent_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetOwner
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::KnowledgeGroup_ABC& AgentKnowledge::GetOwner() const
{
    return knowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Display
// Created: SBO 2008-07-11
// -----------------------------------------------------------------------------
void AgentKnowledge::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}
