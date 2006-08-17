// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_Barrer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Barrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Barrer::Mission_Pawn_ABC_Barrer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_Barrer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Barrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Barrer::~Mission_Pawn_ABC_Barrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Barrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Barrer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_Barrer& asnMission = *new ASN1T_Mission_Pion_ABC_Barrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_barrer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_barrer = &asnMission;

    const Position& positionInstallation_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionInstallation_, asnMission.position_installation );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Barrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Barrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_barrer );
    ASN1T_Mission_Pion_ABC_Barrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_barrer;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


