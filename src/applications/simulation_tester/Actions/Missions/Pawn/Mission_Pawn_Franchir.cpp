// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Franchir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Franchir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Franchir::Mission_Pawn_Franchir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Franchir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Franchir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Franchir::~Mission_Pawn_Franchir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Franchir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Franchir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Franchir& asnMission = *new ASN1T_Mission_Pion_Franchir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_franchir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_franchir = &asnMission;

    const Position& pointArrivee_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.pont_flottant );
    ASN_Tools::CopyPoint( pointArrivee_, asnMission.point_arrivee );

    delete &pointArrivee_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Franchir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Franchir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_franchir );
    ASN1T_Mission_Pion_Franchir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_franchir;

    ASN_Tools::Delete( asnMission.point_arrivee );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


