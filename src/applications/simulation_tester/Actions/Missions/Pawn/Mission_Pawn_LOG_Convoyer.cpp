// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_Convoyer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Convoyer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Convoyer::Mission_Pawn_LOG_Convoyer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_Convoyer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Convoyer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Convoyer::~Mission_Pawn_LOG_Convoyer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Convoyer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Convoyer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *new ASN1T_Mission_Pion_LOG_Convoyer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_convoyer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_convoyer = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Convoyer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Convoyer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_convoyer );
    ASN1T_Mission_Pion_LOG_Convoyer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_convoyer;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


