// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_RENS_ROEM_Appuyer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_Appuyer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_Appuyer::Mission_Pawn_RENS_ROEM_Appuyer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_ROEM_Appuyer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_Appuyer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_Appuyer::~Mission_Pawn_RENS_ROEM_Appuyer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_Appuyer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_Appuyer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_ROEM_Appuyer& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_Appuyer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_appuyer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_appuyer = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );

    delete &positionDeploiement_;
    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_Appuyer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_Appuyer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_roem_appuyer );
    ASN1T_Mission_Pion_RENS_ROEM_Appuyer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_appuyer;

    ASN_Tools::Delete( asnMission.position_deploiement );
    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


