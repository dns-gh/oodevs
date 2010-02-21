// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Harceler.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Harceler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Harceler::Mission_Automat_INF_Harceler( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Harceler", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Harceler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Harceler::~Mission_Automat_INF_Harceler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Harceler::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Harceler::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_Harceler& asnMission = *new Mission_Automate_INF_Harceler();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_harceler = &asnMission;

    const Location& zoneSurveillance_ = pTarget_->GetTestParam_Polygon();
    const Location& pointRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneSurveillance_, asnMission.zone_surveillance );
    ASN_Tools::CopyPolygon( pointRegroupement_, asnMission.point_regroupement );

    delete &zoneSurveillance_;
    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Harceler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Harceler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_harceler ());
    Mission_Automate_INF_Harceler& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_harceler;

    ASN_Tools::Delete( asnMission.zone_surveillance );
    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




