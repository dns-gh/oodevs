// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_ReconnaitreSiteFranchissement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreSiteFranchissement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReconnaitreSiteFranchissement::Mission_Pawn_GEN_ReconnaitreSiteFranchissement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ReconnaitreSiteFranchissement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreSiteFranchissement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReconnaitreSiteFranchissement::~Mission_Pawn_GEN_ReconnaitreSiteFranchissement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreSiteFranchissement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReconnaitreSiteFranchissement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *new Mission_Pion_GEN_ReconnaitreSiteFranchissement();

    asnMsg_.GetAsnMsg().mission().mission_pion_gen_reconnaitre_site_franchissement = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_franchissement );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReconnaitreSiteFranchissement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReconnaitreSiteFranchissement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_gen_reconnaitre_site_franchissement ());
    Mission_Pion_GEN_ReconnaitreSiteFranchissement& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_gen_reconnaitre_site_franchissement;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


