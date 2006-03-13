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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_CanaliserPopulations";
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

    ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *new ASN1T_Mission_Automate_CanaliserPopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_canaliser_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_canaliser_populations = &asnMission;

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
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_canaliser_populations );
    ASN1T_Mission_Automate_CanaliserPopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_canaliser_populations;

    ASN_Tools::Delete( asnMission.zones );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




