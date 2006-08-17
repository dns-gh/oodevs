// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASA_Surveiller.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_Surveiller::Mission_Pawn_ASA_Surveiller( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_Surveiller", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_Surveiller::~Mission_Pawn_ASA_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_Surveiller();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_surveiller;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_surveiller = &asnMission;

    const Position& pointDeDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointDeDeploiement_, asnMission.point_de_deploiement );

    delete &pointDeDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_surveiller );
    ASN1T_Mission_Pion_ASA_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_surveiller;

    ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


