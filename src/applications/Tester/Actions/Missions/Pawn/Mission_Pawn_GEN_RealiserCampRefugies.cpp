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
#include "Mission_Pawn_GEN_RealiserCampRefugies.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampRefugies constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserCampRefugies::Mission_Pawn_GEN_RealiserCampRefugies( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserCampRefugies";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampRefugies destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserCampRefugies::~Mission_Pawn_GEN_RealiserCampRefugies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampRefugies::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserCampRefugies::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampRefugies();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_camp_refugies;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_refugies = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.camp );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.tc2 );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserCampRefugies::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserCampRefugies::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_camp_refugies );
    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_camp_refugies;

    ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


