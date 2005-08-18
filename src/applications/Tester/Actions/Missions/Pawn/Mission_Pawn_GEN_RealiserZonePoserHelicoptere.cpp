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
#include "Mission_Pawn_GEN_RealiserZonePoserHelicoptere.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZonePoserHelicoptere constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZonePoserHelicoptere::Mission_Pawn_GEN_RealiserZonePoserHelicoptere( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserZonePoserHelicoptere";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZonePoserHelicoptere destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserZonePoserHelicoptere::~Mission_Pawn_GEN_RealiserZonePoserHelicoptere()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZonePoserHelicoptere::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZonePoserHelicoptere::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_poser_helicoptere = &asnMission;

    const Position& zonePoser_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( zonePoser_, asnMission.zone_poser );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );

    delete &zonePoser_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserZonePoserHelicoptere::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserZonePoserHelicoptere::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_zone_poser_helicoptere );
    ASN1T_Mission_Pion_GEN_RealiserZonePoserHelicoptere& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_zone_poser_helicoptere;

    ASN_Tools::Delete( asnMission.zone_poser );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


