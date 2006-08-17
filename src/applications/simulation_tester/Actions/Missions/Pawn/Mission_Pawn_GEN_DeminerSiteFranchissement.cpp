// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_DeminerSiteFranchissement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DeminerSiteFranchissement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DeminerSiteFranchissement::Mission_Pawn_GEN_DeminerSiteFranchissement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_DeminerSiteFranchissement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DeminerSiteFranchissement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DeminerSiteFranchissement::~Mission_Pawn_GEN_DeminerSiteFranchissement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DeminerSiteFranchissement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DeminerSiteFranchissement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *new ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_deminer_site_franchissement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_deminer_site_franchissement = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_franchissement );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DeminerSiteFranchissement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DeminerSiteFranchissement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_deminer_site_franchissement );
    ASN1T_Mission_Pion_GEN_DeminerSiteFranchissement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_deminer_site_franchissement;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


