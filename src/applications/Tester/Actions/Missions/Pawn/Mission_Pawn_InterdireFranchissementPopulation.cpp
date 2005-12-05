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
#include "Mission_Pawn_InterdireFranchissementPopulation.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_InterdireFranchissementPopulation::Mission_Pawn_InterdireFranchissementPopulation( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_InterdireFranchissementPopulation";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_InterdireFranchissementPopulation::~Mission_Pawn_InterdireFranchissementPopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_InterdireFranchissementPopulation::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_InterdireFranchissementPopulation& asnMission = *new ASN1T_Mission_Pion_InterdireFranchissementPopulation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_interdire_franchissement_population;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_interdire_franchissement_population = &asnMission;

    const Location& ligne_ = pTarget_->GetTestParam_Location();

    ASN_Tools::CopyLocation( ligne_, asnMission.ligne );

    delete &ligne_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_InterdireFranchissementPopulation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_InterdireFranchissementPopulation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_interdire_franchissement_population );
    ASN1T_Mission_Pion_InterdireFranchissementPopulation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_interdire_franchissement_population;

    ASN_Tools::Delete( asnMission.ligne );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


