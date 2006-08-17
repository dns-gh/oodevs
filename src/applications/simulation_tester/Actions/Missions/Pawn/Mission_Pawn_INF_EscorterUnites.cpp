// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_INF_EscorterUnites.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_EscorterUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_EscorterUnites::Mission_Pawn_INF_EscorterUnites( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_EscorterUnites", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_EscorterUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_EscorterUnites::~Mission_Pawn_INF_EscorterUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_EscorterUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_EscorterUnites::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_EscorterUnites& asnMission = *new ASN1T_Mission_Pion_INF_EscorterUnites();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_escorter_unites;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_escorter_unites = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.automate_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_EscorterUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_EscorterUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_escorter_unites );
    ASN1T_Mission_Pion_INF_EscorterUnites& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_escorter_unites;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


