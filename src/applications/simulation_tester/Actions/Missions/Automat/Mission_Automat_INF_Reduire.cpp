// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Reduire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Reduire::Mission_Automat_INF_Reduire( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Reduire", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Reduire::~Mission_Automat_INF_Reduire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Reduire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_Reduire& asnMission = *new Mission_Automate_INF_Reduire();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_reduire = &asnMission;

    const T_PositionVector& pointsResistance_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( pointsResistance_, asnMission.points_resistance );

    delete &pointsResistance_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Reduire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_reduire ());
    Mission_Automate_INF_Reduire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_reduire;

    ASN_Tools::Delete( asnMission.points_resistance );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



