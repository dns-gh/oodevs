// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserTravauxAppuiDeploiement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::~Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_appui_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_appui_deploiement = &asnMission;

    const T_GenObjectVector& travaux_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( travaux_, asnMission.travaux );

    delete &travaux_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxAppuiDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_appui_deploiement );
    ASN1T_Mission_Pion_GEN_RealiserTravauxAppuiDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_appui_deploiement;

    ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


