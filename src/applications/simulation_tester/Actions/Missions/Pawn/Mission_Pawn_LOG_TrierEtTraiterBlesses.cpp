// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_TrierEtTraiterBlesses.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TrierEtTraiterBlesses::Mission_Pawn_LOG_TrierEtTraiterBlesses( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_TrierEtTraiterBlesses", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TrierEtTraiterBlesses::~Mission_Pawn_LOG_TrierEtTraiterBlesses()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TrierEtTraiterBlesses::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *new Mission_Pion_LOG_TrierEtTraiterBlesses();

    asnMsg_.GetAsnMsg().mission().mission_pion_log_trier_et_traiter_blesses = &asnMission;

    const T_IdVector& blessuresTraitees_ = pTarget_->GetTestParam_MedicalPriorities();
    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyMedicalPriorities( blessuresTraitees_, asnMission.blessures_traitees );
    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &blessuresTraitees_;
    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TrierEtTraiterBlesses::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_log_trier_et_traiter_blesses ());
    Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_log_trier_et_traiter_blesses;

    ASN_Tools::Delete( asnMission.blessures_traitees );
    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


