// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_FaireMouvement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvement::Mission_Automat_FaireMouvement( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_FaireMouvement", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvement::~Mission_Automat_FaireMouvement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_FaireMouvement& asnMission = *new ASN1T_Mission_Automate_FaireMouvement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_faire_mouvement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_faire_mouvement = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_faire_mouvement );
    ASN1T_Mission_Automate_FaireMouvement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_faire_mouvement;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




