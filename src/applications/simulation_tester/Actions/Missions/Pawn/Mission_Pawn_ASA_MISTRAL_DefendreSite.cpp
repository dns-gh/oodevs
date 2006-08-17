// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASA_MISTRAL_DefendreSite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreSite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_DefendreSite::Mission_Pawn_ASA_MISTRAL_DefendreSite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_MISTRAL_DefendreSite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreSite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_DefendreSite::~Mission_Pawn_ASA_MISTRAL_DefendreSite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreSite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_DefendreSite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_site;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site = &asnMission;

    const Location& site_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( site_, asnMission.site );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.mode_deploiement );

    delete &site_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreSite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_DefendreSite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_mistral_defendre_site );
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreSite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_site;

    ASN_Tools::Delete( asnMission.site );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


