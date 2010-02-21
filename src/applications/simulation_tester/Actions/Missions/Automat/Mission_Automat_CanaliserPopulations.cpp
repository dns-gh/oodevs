// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_CanaliserPopulations.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_CanaliserPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_CanaliserPopulations::Mission_Automat_CanaliserPopulations( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_CanaliserPopulations", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_CanaliserPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_CanaliserPopulations::~Mission_Automat_CanaliserPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_CanaliserPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_CanaliserPopulations::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_CanaliserPopulations& asnMission = *new Mission_Automate_CanaliserPopulations();

    asnMsg_.GetAsnMsg().mission().mission_automate_canaliser_populations = &asnMission;

    const T_LocationVector& zones_ = pTarget_->GetTestParam_LocationList();

    ASN_Tools::CopyLocationList( zones_, asnMission.zones );

    delete &zones_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_CanaliserPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_CanaliserPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_canaliser_populations ());
    Mission_Automate_CanaliserPopulations& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_canaliser_populations;

    ASN_Tools::Delete( asnMission.zones );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




