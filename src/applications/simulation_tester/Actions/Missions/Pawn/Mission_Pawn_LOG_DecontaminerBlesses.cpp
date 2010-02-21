// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_DecontaminerBlesses.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DecontaminerBlesses constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DecontaminerBlesses::Mission_Pawn_LOG_DecontaminerBlesses( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_DecontaminerBlesses", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DecontaminerBlesses destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DecontaminerBlesses::~Mission_Pawn_LOG_DecontaminerBlesses()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DecontaminerBlesses::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DecontaminerBlesses::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_LOG_DecontaminerBlesses& asnMission = *new Mission_Pion_LOG_DecontaminerBlesses();

    asnMsg_.GetAsnMsg().mission().mission_pion_log_decontaminer_blesses = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DecontaminerBlesses::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DecontaminerBlesses::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_log_decontaminer_blesses ());
    Mission_Pion_LOG_DecontaminerBlesses& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_log_decontaminer_blesses;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


