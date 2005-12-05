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
#include "Mission_Pawn_CanaliserPopulation.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_CanaliserPopulation::Mission_Pawn_CanaliserPopulation( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_CanaliserPopulation";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_CanaliserPopulation::~Mission_Pawn_CanaliserPopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_CanaliserPopulation::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_CanaliserPopulation& asnMission = *new ASN1T_Mission_Pion_CanaliserPopulation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_canaliser_population;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_canaliser_population = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CanaliserPopulation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_CanaliserPopulation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_canaliser_population );
    ASN1T_Mission_Pion_CanaliserPopulation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_canaliser_population;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


