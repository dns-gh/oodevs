// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_RetablirItineraires.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RetablirItineraires constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RetablirItineraires::Mission_Automat_GEN_RetablirItineraires( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RetablirItineraires", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RetablirItineraires destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RetablirItineraires::~Mission_Automat_GEN_RetablirItineraires()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RetablirItineraires::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RetablirItineraires::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *new ASN1T_Mission_Automate_GEN_RetablirItineraires();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_retablir_itineraires;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_retablir_itineraires = &asnMission;

    const T_PathVector& itineraires_ = pTarget_->GetTestParam_PathList();

    ASN_Tools::CopyPathList( itineraires_, asnMission.itineraires );

    delete &itineraires_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RetablirItineraires::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RetablirItineraires::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_retablir_itineraires );
    ASN1T_Mission_Automate_GEN_RetablirItineraires& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_retablir_itineraires;

    ASN_Tools::Delete( asnMission.itineraires );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




