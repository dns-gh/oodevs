// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_RealiserZoneMineeParDispersion.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeParDispersion constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZoneMineeParDispersion::Mission_Pawn_GEN_RealiserZoneMineeParDispersion( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserZoneMineeParDispersion";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeParDispersion destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZoneMineeParDispersion::~Mission_Pawn_GEN_RealiserZoneMineeParDispersion()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeParDispersion::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZoneMineeParDispersion::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.densite );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZoneMineeParDispersion::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZoneMineeParDispersion::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_zone_minee_par_dispersion );
    ASN1T_Mission_Pion_GEN_RealiserZoneMineeParDispersion& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_minee_par_dispersion;

    ASN_Tools::Delete( asnMission.pos_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


