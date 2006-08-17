// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASY_SInfiltrer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_SInfiltrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_SInfiltrer::Mission_Pawn_ASY_SInfiltrer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASY_SInfiltrer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_SInfiltrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_SInfiltrer::~Mission_Pawn_ASY_SInfiltrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_SInfiltrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_SInfiltrer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASY_SInfiltrer& asnMission = *new ASN1T_Mission_Pion_ASY_SInfiltrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_sinfiltrer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_sinfiltrer = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_SInfiltrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_SInfiltrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asy_sinfiltrer );
    ASN1T_Mission_Pion_ASY_SInfiltrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_sinfiltrer;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


