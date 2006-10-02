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

#include "KnowledgeGroup.h"
#include "Agent.h"
#include "Network_Def.h"
#include "Model.h"
#include "Side.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
AgentKnowledge::AgentKnowledge( Model& model, const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
    : model_                ( model )
    , nID_                  ( asnMsg.oid_connaissance )
    , knowledgeGroup_       ( model.GetKnowledgeGroups().Get( asnMsg.oid_groupe_possesseur ) )
    , agent_                ( model.GetAgents().Get( asnMsg.oid_unite_reelle ) )
    , nRelevance_           ( 0 )
    , nPerceptionLevel_     ( EnumUnitIdentificationLevel::signale )
    , nMaxPerceptionLevel_  ( EnumUnitIdentificationLevel::signale )
    , nOperationalState_    ( 0 )
    , bDead_                ( false )
    , position_             ()
    , nDirection_           ( 0 )
    , nSpeed_               ( 0 )
    , pSide_                ( 0 )
    , nNatureWeapon_        ( (ASN1T_EnumUnitNatureWeapon)-1 )
    , nNatureLevel_         ( (ASN1T_EnumNatureLevel)-1 )
    , nNatureSpecialization_( (ASN1T_EnumUnitNatureSpecialization)-1 )
    , nNatureQualifier_     ( (ASN1T_EnumUnitNatureQualifier)-1 )
    , nNatureCategory_      ( (ASN1T_EnumUnitNatureCategory)-1 )
    , nNatureMobility_      ( (ASN1T_EnumUnitNatureMobility)-1 )
    , nMissionCapability_   ( (ASN1T_EnumUnitCapaciteMission)-1 )
    , bPC_                  ( false )
    , automatePerceptions_  ( )
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
    optionals_.nature_armePresent              = 0;
    optionals_.nature_niveauPresent            = 0;
    optionals_.nature_pcPresent                = 0;
    optionals_.nature_specialisationPresent    = 0;
    optionals_.nature_qualificationPresent     = 0;
    optionals_.nature_categoriePresent         = 0;
    optionals_.nature_mobilitePresent          = 0;
    optionals_.capacite_missionPresent         = 0;
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

}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    UPDATE_ASN_ATTRIBUTE( position                 , position_              );
    UPDATE_ASN_ATTRIBUTE( direction                , nDirection_            );
    UPDATE_ASN_ATTRIBUTE( speed                    , nSpeed_                );
    
    if( asnMsg.m.campPresent )
    {
        optionals_.campPresent = 1;
        pSide_ = &model_.GetSides().Get( asnMsg.camp );
    }
        
    UPDATE_ASN_ATTRIBUTE( nature_arme              , nNatureWeapon_         );
    UPDATE_ASN_ATTRIBUTE( nature_niveau            , nNatureLevel_          );
    UPDATE_ASN_ATTRIBUTE( nature_pc                , bPC_                   ); 
    UPDATE_ASN_ATTRIBUTE( nature_specialisation    , nNatureSpecialization_ );
    UPDATE_ASN_ATTRIBUTE( nature_qualification     , nNatureQualifier_      );
    UPDATE_ASN_ATTRIBUTE( nature_categorie         , nNatureCategory_       );
    UPDATE_ASN_ATTRIBUTE( nature_mobilite          , nNatureMobility_       );
    UPDATE_ASN_ATTRIBUTE( capacite_mission         , nMissionCapability_    );

    if( asnMsg.m.perception_par_compagniePresent )
    {
        optionals_.perception_par_compagniePresent = 1;
        automatePerceptions_.clear();
        for( uint i = 0; i < asnMsg.perception_par_compagnie.n; ++i )
            automatePerceptions_.push_back( asnMsg.perception_par_compagnie.elem[ i ] );
    }

    UPDATE_ASN_ATTRIBUTE( rendu                    , bDead_                 );
    UPDATE_ASN_ATTRIBUTE( prisonnier               , bPrisoner_             );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte   , bRefugeeManaged_       );

}

// -----------------------------------------------------------------------------
// Name: AgentKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void AgentKnowledge::SendCreation( Publisher_ABC& publisher )
{
    AsnMsgInClientUnitKnowledgeCreation asn;

    asn().oid_connaissance      = nID_;
    asn().oid_groupe_possesseur = knowledgeGroup_.GetID();
    asn().oid_unite_reelle      = agent_.GetID();

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
void AgentKnowledge::SendFullUpdate( Publisher_ABC& publisher ) const
{
    AsnMsgInClientUnitKnowledgeUpdate asn;

    asn().oid_connaissance      = nID_;
    asn().oid_groupe_possesseur = knowledgeGroup_.GetID();

    SEND_ASN_ATTRIBUTE( pertinence               , nRelevance_            ); 
    SEND_ASN_ATTRIBUTE( identification_level     , nPerceptionLevel_      );
    SEND_ASN_ATTRIBUTE( max_identification_level , nMaxPerceptionLevel_   );
    SEND_ASN_ATTRIBUTE( etat_op                  , nOperationalState_     );
    SEND_ASN_ATTRIBUTE( mort                     , bDead_                 );
    
    if( optionals_.positionPresent )
    {
        asn().m.positionPresent = 1;
        position_.Send( asn().position );
    }

    SEND_ASN_ATTRIBUTE( direction                , nDirection_            );
    SEND_ASN_ATTRIBUTE( speed                    , nSpeed_                );
    
    if( optionals_.campPresent )
    {
        assert( pSide_ );
        asn().m.campPresent = 1;
        asn().camp = pSide_->GetID();
    }
        
    SEND_ASN_ATTRIBUTE( nature_arme              , nNatureWeapon_         );
    SEND_ASN_ATTRIBUTE( nature_niveau            , nNatureLevel_          );
    SEND_ASN_ATTRIBUTE( nature_pc                , bPC_                   ); 
    SEND_ASN_ATTRIBUTE( nature_specialisation    , nNatureSpecialization_ );
    SEND_ASN_ATTRIBUTE( nature_qualification     , nNatureQualifier_      );
    SEND_ASN_ATTRIBUTE( nature_categorie         , nNatureCategory_       );
    SEND_ASN_ATTRIBUTE( nature_mobilite          , nNatureMobility_       );
    SEND_ASN_ATTRIBUTE( capacite_mission         , nMissionCapability_    );

    if( optionals_.perception_par_compagniePresent )
    {
        asn().m.perception_par_compagniePresent = 1;
        asn().perception_par_compagnie.n = automatePerceptions_.size();
        if( !automatePerceptions_.empty() )
        {
            asn().perception_par_compagnie.elem = new ASN1T_AutomatePerception[ automatePerceptions_.size() ];
            for( uint i = 0; i < automatePerceptions_.size(); ++i )
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
