// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_ArmerPointDeControle.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPointDeControle constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ArmerPointDeControle::Mission_Pawn_ABC_ArmerPointDeControle( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_ArmerPointDeControle", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPointDeControle destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ArmerPointDeControle::~Mission_Pawn_ABC_ArmerPointDeControle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPointDeControle::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ArmerPointDeControle::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPointDeControle();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_armer_point_de_controle;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle = &asnMission;

    const Position& pointDeControle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointDeControle_, asnMission.point_de_controle );

    delete &pointDeControle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ArmerPointDeControle::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ArmerPointDeControle::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_armer_point_de_controle );
    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_armer_point_de_controle;

    ASN_Tools::Delete( asnMission.point_de_controle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


