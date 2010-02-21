// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_InterdireFranchissementPopulations.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_InterdireFranchissementPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_InterdireFranchissementPopulations::Mission_Automat_InterdireFranchissementPopulations( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_InterdireFranchissementPopulations", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_InterdireFranchissementPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_InterdireFranchissementPopulations::~Mission_Automat_InterdireFranchissementPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_InterdireFranchissementPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_InterdireFranchissementPopulations::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_InterdireFranchissementPopulations& asnMission = *new Mission_Automate_InterdireFranchissementPopulations();

    asnMsg_.GetAsnMsg().mission().mission_automate_interdire_franchissement_populations = &asnMission;

    const T_PositionVector& points_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( points_, asnMission.points );

    delete &points_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_InterdireFranchissementPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_InterdireFranchissementPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_interdire_franchissement_populations ());
    Mission_Automate_InterdireFranchissementPopulations& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_interdire_franchissement_populations;

    ASN_Tools::Delete( asnMission.points );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




