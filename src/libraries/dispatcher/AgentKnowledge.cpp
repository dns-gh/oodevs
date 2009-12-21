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
#include "Model.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "ClientPublisher_ABC.h"
#include "Agent.h"
#include "KnowledgeGroup.h"
#include "Side.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( Model& model, const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
    : SimpleEntity< kernel::AgentKnowledge_ABC >( asnMsg.oid )
    , model_                ( model )
    , knowledgeGroup_       ( model.knowledgeGroups_.Get( asnMsg.oid_groupe_possesseur ) )
    , agent_                ( model.agents_.Get( asnMsg.oid_unite_reelle ) )
    , type_                 ( asnMsg.type_unite )
    , nRelevance_           ( 0 )
    , nPerceptionLevel_     ( EnumUnitIdentificationLevel::signale )
    , nMaxPerceptionLevel_  ( EnumUnitIdentificationLevel::signale )
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
    //$$ BULLSHIT
    optionals_.pertinencePresent               = 0;
    optionals_.identification_levelPresent     = 0;
    optionals_.max_identification_levelPresent = 0;
    optionals_.etat_opPresent                  = 0;
    optionals_.mortPresent                     = 0;
    optionals_.positionPresent                 = 0;
    optionals_.directionPresent                = 0;
    optionals_.speedPresent                    = 0;
    optionals_.campPresent                     = 0;
    optionals_.nature_pcPresent                = 0;
    optionals_.perception_par_compagniePresent = 0;
    optionals_.renduPresent                    = 0;
    optionals_.prisonnierPresent               = 0;
    optionals_.refugie_pris_en_comptePresent   = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge destructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::~AgentKnowledge()
{
    // NOTHING
}

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( asnMsg.m.##ASN##Present )        \
    {                                    \
        CPP = asnMsg.##ASN;              \
        optionals_.##ASN##Present = 1;   \
    }

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    UPDATE_ASN_ATTRIBUTE( pertinence               , nRelevance_            );
    UPDATE_ASN_ATTRIBUTE( identification_level     , nPerceptionLevel_      );
    UPDATE_ASN_ATTRIBUTE( max_identification_level , nMaxPerceptionLevel_   );
    UPDATE_ASN_ATTRIBUTE( etat_op                  , nOperationalState_     );
    UPDATE_ASN_ATTRIBUTE( mort                     , bDead_                 );
    if( asnMsg.m.positionPresent )
        position_.Set( asnMsg.position.latitude, asnMsg.position.longitude );
    UPDATE_ASN_ATTRIBUTE( direction                , nDirection_            );
    UPDATE_ASN_ATTRIBUTE( speed                    , nSpeed_                );

    if( asnMsg.m.campPresent )
    {
        optionals_.campPresent = 1;
        team_ = &model_.sides_.Get( asnMsg.camp );
    }

    UPDATE_ASN_ATTRIBUTE( nature_pc, bPC_ );

    if( asnMsg.m.perception_par_compagniePresent )
    {
        optionals_.perception_par_compagniePresent = 1;
        automatePerceptions_.clear();
        for( unsigned int i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
            automatePerceptions_.push_back( asnMsg.perception_par_compagnie.elem[ i ] );
    }

    UPDATE_ASN_ATTRIBUTE( rendu                    , bDead_                 );
    UPDATE_ASN_ATTRIBUTE( prisonnier               , bPrisoner_             );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte   , bRefugeeManaged_       );

    ApplyUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeCreation asn;

    asn().oid                   = GetId();
    asn().oid_groupe_possesseur = knowledgeGroup_.GetId();
    asn().oid_unite_reelle      = agent_.GetId();
    asn().type_unite            = type_;

    asn.Send( publisher );
}

#define SEND_ASN_ATTRIBUTE( ASN, CPP )  \
    if( optionals_.##ASN##Present )     \
    {                                   \
        asn().m.##ASN##Present = 1;     \
        asn().##ASN = CPP;              \
    }

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeUpdate asn;

    asn().oid                   = GetId();
    asn().oid_groupe_possesseur = knowledgeGroup_.GetId();

    SEND_ASN_ATTRIBUTE( pertinence               , nRelevance_            );
    SEND_ASN_ATTRIBUTE( identification_level     , nPerceptionLevel_      );
    SEND_ASN_ATTRIBUTE( max_identification_level , nMaxPerceptionLevel_   );
    SEND_ASN_ATTRIBUTE( etat_op                  , nOperationalState_     );
    SEND_ASN_ATTRIBUTE( mort                     , bDead_                 );

    if( optionals_.positionPresent )
    {
        asn().m.positionPresent = 1;
        asn().position.latitude = position_.X();
        asn().position.longitude = position_.Y();
    }

    SEND_ASN_ATTRIBUTE( direction, nDirection_ );
    SEND_ASN_ATTRIBUTE( speed    , nSpeed_     );

    if( optionals_.campPresent )
    {
        assert( team_ );
        asn().m.campPresent = 1;
        asn().camp = team_->GetId();
    }

    SEND_ASN_ATTRIBUTE( nature_pc, bPC_ );

    if( optionals_.perception_par_compagniePresent )
    {
        asn().m.perception_par_compagniePresent = 1;
        asn().perception_par_compagnie.n = automatePerceptions_.size();
        if( !automatePerceptions_.empty() )
        {
            asn().perception_par_compagnie.elem = new ASN1T_AutomatPerception[ automatePerceptions_.size() ];
            for( unsigned int i = 0; i < automatePerceptions_.size(); ++i )
                asn().perception_par_compagnie.elem[ i ] = automatePerceptions_[ i ];
        }
    }

    SEND_ASN_ATTRIBUTE( rendu                    , bDead_                 );
    SEND_ASN_ATTRIBUTE( prisonnier               , bPrisoner_             );
    SEND_ASN_ATTRIBUTE( refugie_pris_en_compte   , bRefugeeManaged_       );


    asn.Send( publisher );

    if( asn().m.perception_par_compagniePresent && asn().perception_par_compagnie.n > 0 )
        delete [] asn().perception_par_compagnie.elem;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void AgentKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UnitKnowledgeDestruction asn;
    asn().oid                   = GetId();
    asn().oid_groupe_possesseur = knowledgeGroup_.GetId();
    asn.Send( publisher );
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
