// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_ReconnaissanceOffensive.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_ReconnaissanceOffensive constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_ReconnaissanceOffensive::Mission_Automat_INF_ReconnaissanceOffensive( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_ReconnaissanceOffensive", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_ReconnaissanceOffensive destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_ReconnaissanceOffensive::~Mission_Automat_INF_ReconnaissanceOffensive()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_ReconnaissanceOffensive::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_ReconnaissanceOffensive::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_ReconnaissanceOffensive& asnMission = *new Mission_Automate_INF_ReconnaissanceOffensive();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_reconnaissance_offensive = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_ReconnaissanceOffensive::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_ReconnaissanceOffensive::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_reconnaissance_offensive ());
    Mission_Automate_INF_ReconnaissanceOffensive& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_reconnaissance_offensive;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



