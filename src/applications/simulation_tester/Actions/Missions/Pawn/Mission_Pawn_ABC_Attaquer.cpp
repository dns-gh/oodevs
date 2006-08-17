// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ABC_Attaquer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Attaquer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Attaquer::Mission_Pawn_ABC_Attaquer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_Attaquer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Attaquer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Attaquer::~Mission_Pawn_ABC_Attaquer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Attaquer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Attaquer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_Attaquer& asnMission = *new ASN1T_Mission_Pion_ABC_Attaquer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_attaquer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_attaquer = &asnMission;

    const Path& itineraireAssaut_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraireAssaut_, asnMission.itineraire_assaut );

    delete &itineraireAssaut_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Attaquer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Attaquer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_attaquer );
    ASN1T_Mission_Pion_ABC_Attaquer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_attaquer;

    ASN_Tools::Delete( asnMission.itineraire_assaut );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


