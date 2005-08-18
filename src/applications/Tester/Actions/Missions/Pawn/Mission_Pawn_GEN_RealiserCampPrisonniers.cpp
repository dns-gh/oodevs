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
#include "Mission_Pawn_GEN_RealiserCampPrisonniers.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampPrisonniers constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserCampPrisonniers::Mission_Pawn_GEN_RealiserCampPrisonniers( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserCampPrisonniers";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampPrisonniers destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserCampPrisonniers::~Mission_Pawn_GEN_RealiserCampPrisonniers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampPrisonniers::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserCampPrisonniers::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_prisonniers = &asnMission;

    const Location& camp_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( camp_, asnMission.camp );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.tc2 );

    delete &camp_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampPrisonniers::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserCampPrisonniers::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers );
    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_prisonniers;

    ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


