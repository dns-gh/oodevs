// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_NBC_DecontaminerZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_DecontaminerZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_DecontaminerZone::Mission_Pawn_NBC_DecontaminerZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_NBC_DecontaminerZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_DecontaminerZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_DecontaminerZone::~Mission_Pawn_NBC_DecontaminerZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_DecontaminerZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_DecontaminerZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_NBC_DecontaminerZone& asnMission = *new ASN1T_Mission_Pion_NBC_DecontaminerZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_decontaminer_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_decontaminer_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_DecontaminerZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_DecontaminerZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_nbc_decontaminer_zone );
    ASN1T_Mission_Pion_NBC_DecontaminerZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_decontaminer_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


