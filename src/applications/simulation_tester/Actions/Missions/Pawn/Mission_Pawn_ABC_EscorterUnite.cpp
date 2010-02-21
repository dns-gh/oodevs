// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_EscorterUnite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_EscorterUnite::Mission_Pawn_ABC_EscorterUnite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_EscorterUnite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_EscorterUnite::~Mission_Pawn_ABC_EscorterUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_EscorterUnite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ABC_EscorterUnite& asnMission = *new Mission_Pion_ABC_EscorterUnite();

    asnMsg_.GetAsnMsg().mission().mission_pion_abc_escorter_unite = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_EscorterUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_abc_escorter_unite ());
    Mission_Pion_ABC_EscorterUnite& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_abc_escorter_unite;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


