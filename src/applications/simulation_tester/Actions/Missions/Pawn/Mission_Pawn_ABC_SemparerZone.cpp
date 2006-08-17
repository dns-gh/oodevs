// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ABC_SemparerZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_SemparerZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_SemparerZone::Mission_Pawn_ABC_SemparerZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_SemparerZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_SemparerZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_SemparerZone::~Mission_Pawn_ABC_SemparerZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_SemparerZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_SemparerZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *new ASN1T_Mission_Pion_ABC_SemparerZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_semparer_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_semparer_zone = &asnMission;

    const Location& zoneAPrendre_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneAPrendre_, asnMission.zone_a_prendre );

    delete &zoneAPrendre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_SemparerZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_SemparerZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_semparer_zone );
    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_semparer_zone;

    ASN_Tools::Delete( asnMission.zone_a_prendre );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


