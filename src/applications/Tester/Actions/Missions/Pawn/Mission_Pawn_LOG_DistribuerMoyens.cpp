// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_LOG_DistribuerMoyens.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerMoyens constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DistribuerMoyens::Mission_Pawn_LOG_DistribuerMoyens( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_DistribuerMoyens", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerMoyens destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DistribuerMoyens::~Mission_Pawn_LOG_DistribuerMoyens()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerMoyens::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DistribuerMoyens::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_DistribuerMoyens& asnMission = *new ASN1T_Mission_Pion_LOG_DistribuerMoyens();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_distribuer_moyens;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_distribuer_moyens = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerMoyens::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DistribuerMoyens::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_distribuer_moyens );
    ASN1T_Mission_Pion_LOG_DistribuerMoyens& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_distribuer_moyens;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


