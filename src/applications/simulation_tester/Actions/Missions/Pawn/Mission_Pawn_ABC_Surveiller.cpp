// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_Surveiller.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Surveiller::Mission_Pawn_ABC_Surveiller( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_Surveiller", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Surveiller::~Mission_Pawn_ABC_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ABC_Surveiller& asnMission = *new Mission_Pion_ABC_Surveiller();

    asnMsg_.GetAsnMsg().mission().mission_pion_abc_surveiller = &asnMission;

    const Position& position_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( position_, asnMission.position );

    delete &position_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_abc_surveiller ());
    Mission_Pion_ABC_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_abc_surveiller;

    ASN_Tools::Delete( asnMission.position );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


