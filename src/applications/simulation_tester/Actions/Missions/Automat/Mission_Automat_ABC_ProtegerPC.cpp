// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_ProtegerPC.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ProtegerPC constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ProtegerPC::Mission_Automat_ABC_ProtegerPC( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_ProtegerPC", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ProtegerPC destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ProtegerPC::~Mission_Automat_ABC_ProtegerPC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ProtegerPC::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ProtegerPC::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_ProtegerPC& asnMission = *new Mission_Automate_ABC_ProtegerPC();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_proteger_pc = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.pc );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ProtegerPC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ProtegerPC::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_proteger_pc ());
    Mission_Automate_ABC_ProtegerPC& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_proteger_pc;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



