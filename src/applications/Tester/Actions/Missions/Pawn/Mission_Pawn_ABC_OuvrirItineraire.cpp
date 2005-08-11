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
#include "Mission_Pawn_ABC_OuvrirItineraire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OuvrirItineraire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_OuvrirItineraire::Mission_Pawn_ABC_OuvrirItineraire( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ABC_OuvrirItineraire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OuvrirItineraire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_OuvrirItineraire::~Mission_Pawn_ABC_OuvrirItineraire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OuvrirItineraire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_OuvrirItineraire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *new ASN1T_Mission_Pion_ABC_OuvrirItineraire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_ouvrir_itineraire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_ouvrir_itineraire = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OuvrirItineraire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_OuvrirItineraire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_ouvrir_itineraire );
    ASN1T_Mission_Pion_ABC_OuvrirItineraire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_ouvrir_itineraire;

    ASN_Tools::Delete( asnMission.itineraire );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


