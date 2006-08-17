// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_Renforcer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Renforcer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Renforcer::Mission_Pawn_LOG_Renforcer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_Renforcer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Renforcer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Renforcer::~Mission_Pawn_LOG_Renforcer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Renforcer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Renforcer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_Renforcer& asnMission = *new ASN1T_Mission_Pion_LOG_Renforcer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_renforcer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_renforcer = &asnMission;

    const Position& lieuRenforcement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( lieuRenforcement_, asnMission.lieu_renforcement );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.automate );

    delete &lieuRenforcement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Renforcer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Renforcer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_renforcer );
    ASN1T_Mission_Pion_LOG_Renforcer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_renforcer;

    ASN_Tools::Delete( asnMission.lieu_renforcement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


