// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_ReleverUnite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReleverUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReleverUnite::Mission_Pawn_ABC_ReleverUnite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_ReleverUnite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReleverUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReleverUnite::~Mission_Pawn_ABC_ReleverUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReleverUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReleverUnite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ABC_ReleverUnite& asnMission = *new Mission_Pion_ABC_ReleverUnite();

    asnMsg_.GetAsnMsg().mission().mission_pion_abc_relever_unite = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_relever );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReleverUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReleverUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_abc_relever_unite ());
    Mission_Pion_ABC_ReleverUnite& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_abc_relever_unite;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


