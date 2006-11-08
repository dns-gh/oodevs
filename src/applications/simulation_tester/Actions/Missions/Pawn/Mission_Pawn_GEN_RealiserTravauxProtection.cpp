// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserTravauxProtection.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxProtection::Mission_Pawn_GEN_RealiserTravauxProtection( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserTravauxProtection", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxProtection::~Mission_Pawn_GEN_RealiserTravauxProtection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxProtection::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection = &asnMission;

    const T_GenObjectVector& travaux_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( travaux_, asnMission.travaux );

    delete &travaux_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxProtection::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_protection );
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection;

    ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


