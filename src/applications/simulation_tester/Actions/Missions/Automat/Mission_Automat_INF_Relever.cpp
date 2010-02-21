// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Relever.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Relever constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Relever::Mission_Automat_INF_Relever( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Relever", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Relever destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Relever::~Mission_Automat_INF_Relever()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Relever::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Relever::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_Relever& asnMission = *new Mission_Automate_INF_Relever();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_relever = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Relever::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Relever::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_relever ());
    Mission_Automate_INF_Relever& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_relever;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




