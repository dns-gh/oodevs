// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASA_MISTRAL_DefendreZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_DefendreZone::Mission_Pawn_ASA_MISTRAL_DefendreZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_MISTRAL_DefendreZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_DefendreZone::~Mission_Pawn_ASA_MISTRAL_DefendreZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_DefendreZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_defendre_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.mode_deploiement );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_DefendreZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_DefendreZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_mistral_defendre_zone );
    ASN1T_Mission_Pion_ASA_MISTRAL_DefendreZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_defendre_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


