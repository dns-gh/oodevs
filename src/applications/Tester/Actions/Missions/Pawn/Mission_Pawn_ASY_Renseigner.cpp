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
#include "Mission_Pawn_ASY_Renseigner.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_Renseigner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_Renseigner::Mission_Pawn_ASY_Renseigner( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ASY_Renseigner";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_Renseigner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_Renseigner::~Mission_Pawn_ASY_Renseigner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_Renseigner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_Renseigner::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASY_Renseigner& asnMission = *new ASN1T_Mission_Pion_ASY_Renseigner();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_renseigner;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_renseigner = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.destinataire );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_Renseigner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_Renseigner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asy_renseigner );
    ASN1T_Mission_Pion_ASY_Renseigner& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_renseigner;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


