// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_INF_Interdire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Interdire::Mission_Automat_INF_Interdire( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Interdire", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Interdire::~Mission_Automat_INF_Interdire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Interdire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Interdire& asnMission = *new ASN1T_Mission_Automate_INF_Interdire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_interdire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_interdire = &asnMission;

    const T_PositionVector& pointsAInterdire_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( pointsAInterdire_, asnMission.points_a_interdire );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &pointsAInterdire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Interdire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_interdire );
    ASN1T_Mission_Automate_INF_Interdire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_interdire;

    ASN_Tools::Delete( asnMission.points_a_interdire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




