// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserZoneMineeLineaire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeLineaire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZoneMineeLineaire::Mission_Pawn_GEN_RealiserZoneMineeLineaire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserZoneMineeLineaire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeLineaire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZoneMineeLineaire::~Mission_Pawn_GEN_RealiserZoneMineeLineaire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeLineaire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZoneMineeLineaire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_lineaire = &asnMission;

    const Location& posObstacle_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( posObstacle_, asnMission.pos_obstacle );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.densite );

    delete &posObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeLineaire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZoneMineeLineaire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_zone_minee_lineaire );
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeLineaire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_lineaire;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


