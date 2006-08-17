// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_JOINT_AIR_EffectuerRaid.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_AIR_EffectuerRaid constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_JOINT_AIR_EffectuerRaid::Mission_Pawn_JOINT_AIR_EffectuerRaid( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_JOINT_AIR_EffectuerRaid", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_AIR_EffectuerRaid destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_JOINT_AIR_EffectuerRaid::~Mission_Pawn_JOINT_AIR_EffectuerRaid()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_AIR_EffectuerRaid::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_JOINT_AIR_EffectuerRaid::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& asnMission = *new ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_joint_air_effectuer_raid;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_joint_air_effectuer_raid = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &itineraire_;
    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_JOINT_AIR_EffectuerRaid::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_JOINT_AIR_EffectuerRaid::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_joint_air_effectuer_raid );
    ASN1T_Mission_Pion_JOINT_AIR_EffectuerRaid& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_joint_air_effectuer_raid;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


