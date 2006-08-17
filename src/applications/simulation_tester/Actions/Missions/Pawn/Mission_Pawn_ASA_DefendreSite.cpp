// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASA_DefendreSite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreSite::Mission_Pawn_ASA_DefendreSite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_DefendreSite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreSite::~Mission_Pawn_ASA_DefendreSite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreSite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_site = &asnMission;

    const Location& site_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( site_, asnMission.site );

    delete &site_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreSite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_defendre_site );
    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_site;

    ASN_Tools::Delete( asnMission.site );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


