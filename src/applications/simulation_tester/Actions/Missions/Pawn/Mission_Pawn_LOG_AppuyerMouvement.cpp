// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_AppuyerMouvement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerMouvement::Mission_Pawn_LOG_AppuyerMouvement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_AppuyerMouvement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerMouvement::~Mission_Pawn_LOG_AppuyerMouvement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerMouvement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_appuyer );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_appuyer_mouvement );
    ASN1T_Mission_Pion_LOG_AppuyerMouvement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


