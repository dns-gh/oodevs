// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_RealiserTravaux.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravaux constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserTravaux::Mission_Automat_GEN_RealiserTravaux( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RealiserTravaux", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravaux destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserTravaux::~Mission_Automat_GEN_RealiserTravaux()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravaux::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserTravaux::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserTravaux& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserTravaux();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_travaux;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux = &asnMission;

    const T_GenObjectVector& travaux_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( travaux_, asnMission.travaux );

    delete &travaux_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravaux::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserTravaux::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_travaux );
    ASN1T_Mission_Automate_GEN_RealiserTravaux& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux;

    ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




