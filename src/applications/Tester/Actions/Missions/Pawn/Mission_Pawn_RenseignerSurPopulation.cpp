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
#include "Mission_Pawn_RenseignerSurPopulation.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RenseignerSurPopulation::Mission_Pawn_RenseignerSurPopulation( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_RenseignerSurPopulation";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RenseignerSurPopulation::~Mission_Pawn_RenseignerSurPopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RenseignerSurPopulation::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RenseignerSurPopulation& asnMission = *new ASN1T_Mission_Pion_RenseignerSurPopulation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_renseigner_sur_population;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_renseigner_sur_population = &asnMission;

    const Position& point_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( point_, asnMission.point );

    delete &point_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RenseignerSurPopulation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RenseignerSurPopulation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_renseigner_sur_population );
    ASN1T_Mission_Pion_RenseignerSurPopulation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_renseigner_sur_population;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


