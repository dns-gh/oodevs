// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_ContreAttaquer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ContreAttaquer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ContreAttaquer::Mission_Pawn_ABC_ContreAttaquer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_ContreAttaquer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ContreAttaquer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ContreAttaquer::~Mission_Pawn_ABC_ContreAttaquer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ContreAttaquer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ContreAttaquer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *new ASN1T_Mission_Pion_ABC_ContreAttaquer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_contre_attaquer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_contre_attaquer = &asnMission;

    const Path& itineraireAssaut_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraireAssaut_, asnMission.itineraire_assaut );

    delete &itineraireAssaut_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ContreAttaquer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ContreAttaquer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_contre_attaquer );
    ASN1T_Mission_Pion_ABC_ContreAttaquer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_contre_attaquer;

    ASN_Tools::Delete( asnMission.itineraire_assaut );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


