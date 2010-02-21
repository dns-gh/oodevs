// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_BarrerDirection.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_BarrerDirection::Mission_Automat_INF_BarrerDirection( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_BarrerDirection", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_BarrerDirection::~Mission_Automat_INF_BarrerDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_BarrerDirection::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_BarrerDirection& asnMission = *new Mission_Automate_INF_BarrerDirection();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_barrer_direction = &asnMission;

    const Location& position_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( position_, asnMission.position );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &position_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_BarrerDirection::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_barrer_direction ());
    Mission_Automate_INF_BarrerDirection& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_barrer_direction;

    ASN_Tools::Delete( asnMission.position );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




