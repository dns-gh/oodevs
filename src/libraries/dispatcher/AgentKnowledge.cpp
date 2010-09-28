// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentKnowledge.h"
#include "Agent_ABC.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Side.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( Model& model, const MsgsSimToClient::MsgUnitKnowledgeCreation& message )
    : dispatcher::AgentKnowledge_ABC( message.knowledge().id() )
    , model_                        ( model )
    , knowledgeGroup_               ( model.KnowledgeGroups().Get( message.knowledge_group().id() ) )
    , agent_                        ( model.Agents().Get( message.unit().id() ) )
    , type_                         ( message.type() )
    , nRelevance_                   ( 0 )
    , nPerceptionLevel_             ( MsgsSimToClient::signale )
    , nMaxPerceptionLevel_          ( MsgsSimToClient::signale )
    , nOperationalState_            ( 0 )
    , bDead_                        ( false )
    , nDirection_                   ( 0 )
    , nSpeed_                       ( 0 )
    , team_                         ( 0 )
    , bPC_                          ( false )
    , surrendered_                  ( 0 )
    , bPrisoner_                    ( false )
    , bRefugeeManaged_              ( false )
{
    optionals_.pertinencePresent = 0;
    optionals_.identification_levelPresent = 0;
    optionals_.max_identification_levelPresent = 0;
    optionals_.etat_opPresent = 0;
    optionals_.mortPresent = 0;
    optionals_.speedPresent = 0;
    optionals_.nature_pcPresent = 0;
    optionals_.renduPresent = 0;
    optionals_.prisonnierPresent = 0;
    optionals_.refugie_pris_en_comptePresent = 0;
    optionals_.positionPresent = 0;
    optionals_.directionPresent = 0;
    optionals_.campPresent = 0;
    optionals_.perception_par_compagniePresent = 0;
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    // NOTHING
}

#define UPDATE_ASN_ATTRIBUTE( MESSAGE, ASN, CPP ) \
    if( ##MESSAGE##.has_##ASN##() )               \
    {                                             \
        CPP = ##MESSAGE##.##ASN();                \
        optionals_.##ASN##Present = 1;             \
    }

#define UPDATE_ASN_ATTRIBUTE_ID(MESSAGE, ASN, CPP ) \
    if( ##MESSAGE##.has_##ASN##() )              \
{                                            \
    CPP = ##MESSAGE##.##ASN().id();               \
}


#define SEND_ASN_ATTRIBUTE( MESSAGE, ASN, CPP )  \
    if( optionals_.##ASN##Present )    \
    {                                   \
        ##MESSAGE##.set_##ASN( CPP );  \
    }

#define SEND_ASN_ATTRIBUTE_MUT( MESSAGE, ASN, CPP )  \
    if ( ##MESSAGE##.has_##ASN##() )    \
{                                   \
##MESSAGE##.mutable_##ASN## ()->set_id( ##CPP## );  \
}   


// -----------------------------------------------------------------------------
// Name: AgentKnowledge::DoUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::DoUpdate( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message )
{
    UPDATE_ASN_ATTRIBUTE( message, pertinence              , nRelevance_          );
    UPDATE_ASN_ATTRIBUTE( message, identification_level    , nPerceptionLevel_    );
    UPDATE_ASN_ATTRIBUTE( message, max_identification_level, nMaxPerceptionLevel_ );
    UPDATE_ASN_ATTRIBUTE( message, etat_op                 , nOperationalState_   );
    UPDATE_ASN_ATTRIBUTE( message, mort                    , bDead_               );
    UPDATE_ASN_ATTRIBUTE( message, speed                   , nSpeed_              );
    UPDATE_ASN_ATTRIBUTE( message, nature_pc               , bPC_                 );
    UPDATE_ASN_ATTRIBUTE( message, prisonnier              , bPrisoner_           );
    UPDATE_ASN_ATTRIBUTE( message, refugie_pris_en_compte  , bRefugeeManaged_     );

    if( message.has_surrendered_unit() )
    {
        surrendered_ = message.surrendered_unit().id();
        optionals_.renduPresent = 1;
    }
    if( message.has_position() )
    {
        position_.Set( message.position().latitude(), message.position().longitude() );
        optionals_.positionPresent = 1;
    }
    if( message.has_direction() )
    {
        nDirection_ = message.direction().heading();
        optionals_.directionPresent = 1;
    }
    if( message.has_party() )
    {
        team_ = &model_.Sides().Get( message.party().id() );
        optionals_.campPresent = 1;
    }
    if( message.has_perception_par_compagnie() )
    {
        automatePerceptions_.clear();
        for( int i = 0; i < message.perception_par_compagnie().elem_size(); ++i )
            automatePerceptions_.push_back( message.perception_par_compagnie().elem().Get(i) );
        optionals_.perception_par_compagniePresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeCreation message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    message().mutable_unit()->set_id( agent_.GetId() );
    message().mutable_type()->set_id( type_.id() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeUpdate message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
    SEND_ASN_ATTRIBUTE( message(), pertinence              , nRelevance_          );
    SEND_ASN_ATTRIBUTE( message(), identification_level    , nPerceptionLevel_    );
    SEND_ASN_ATTRIBUTE( message(), max_identification_level, nMaxPerceptionLevel_ );
    SEND_ASN_ATTRIBUTE( message(), etat_op                 , nOperationalState_   );
    SEND_ASN_ATTRIBUTE( message(), mort                    , bDead_               );
    SEND_ASN_ATTRIBUTE( message(), speed                   , nSpeed_              );
    SEND_ASN_ATTRIBUTE( message(), nature_pc               , bPC_                 );
    SEND_ASN_ATTRIBUTE( message(), prisonnier              , bPrisoner_           );
    SEND_ASN_ATTRIBUTE( message(), refugie_pris_en_compte  , bRefugeeManaged_     );
    if( optionals_.renduPresent )
        message().mutable_surrendered_unit()->set_id( surrendered_ );
    if( optionals_.positionPresent )
    {
        message().mutable_position()->set_latitude( position_.X() );
        message().mutable_position()->set_longitude( position_.Y() );
    }
    if( optionals_.directionPresent )
        message().mutable_direction()->set_heading( nDirection_ );
    if( team_ && optionals_.campPresent )
        message().mutable_party()->set_id( team_->GetId() );
    if( optionals_.perception_par_compagniePresent )
        for( unsigned int i = 0; i < automatePerceptions_.size(); ++i )
            *message().mutable_perception_par_compagnie()->add_elem() = automatePerceptions_[ i ];
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void AgentKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeDestruction message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_knowledge_group()->set_id( knowledgeGroup_.GetId() );
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

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetMaxPerceptionLevel
// Created: SBO 2010-06-08
// -----------------------------------------------------------------------------
MsgsSimToClient::EnumUnitIdentificationLevel AgentKnowledge::GetMaxPerceptionLevel() const
{
    return nMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::GetPosition
// Created: SBO 2010-06-08
// -----------------------------------------------------------------------------
geometry::Point2d AgentKnowledge::GetPosition() const
{
    return position_;
}
