// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_INF_FlancGarder.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_FlancGarder constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_FlancGarder::Mission_Automat_INF_FlancGarder( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_FlancGarder", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_FlancGarder destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_FlancGarder::~Mission_Automat_INF_FlancGarder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_FlancGarder::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_FlancGarder::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_FlancGarder& asnMission = *new ASN1T_Mission_Automate_INF_FlancGarder();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_flanc_garder;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_flanc_garder = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_FlancGarder::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_FlancGarder::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_flanc_garder );
    ASN1T_Mission_Automate_INF_FlancGarder& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_flanc_garder;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




