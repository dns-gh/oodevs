// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_Reduire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Reduire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Reduire::Mission_Pawn_ABC_Reduire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_Reduire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Reduire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Reduire::~Mission_Pawn_ABC_Reduire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Reduire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Reduire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ABC_Reduire& asnMission = *new Mission_Pion_ABC_Reduire();

    asnMsg_.GetAsnMsg().mission().mission_pion_abc_reduire = &asnMission;


    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.unite_a_reduire );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Reduire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Reduire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_abc_reduire ());
    Mission_Pion_ABC_Reduire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_abc_reduire;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


