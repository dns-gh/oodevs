// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Pawn_GEN_RetablirItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RetablirItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RetablirItineraire::Mission_Pawn_GEN_RetablirItineraire( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_RetablirItineraire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RetablirItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RetablirItineraire::~Mission_Pawn_GEN_RetablirItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RetablirItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RetablirItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RetablirItineraire& asnMission = *new ASN1T_Mission_Pion_GEN_RetablirItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_retablir_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_retablir_itineraire = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RetablirItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RetablirItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_retablir_itineraire );
    ASN1T_Mission_Pion_GEN_RetablirItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_retablir_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


