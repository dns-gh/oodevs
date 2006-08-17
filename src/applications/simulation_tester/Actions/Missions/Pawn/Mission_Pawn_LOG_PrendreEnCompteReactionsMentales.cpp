// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_PrendreEnCompteReactionsMentales.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PrendreEnCompteReactionsMentales constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::Mission_Pawn_LOG_PrendreEnCompteReactionsMentales( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_PrendreEnCompteReactionsMentales", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PrendreEnCompteReactionsMentales destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::~Mission_Pawn_LOG_PrendreEnCompteReactionsMentales()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *new ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_PrendreEnCompteReactionsMentales::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_prendre_en_compte_reactions_mentales );
    ASN1T_Mission_Pion_LOG_PrendreEnCompteReactionsMentales& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_prendre_en_compte_reactions_mentales;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


