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
#include "Mission_Pawn_GEN_OuvrirTrouee.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirTrouee constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_OuvrirTrouee::Mission_Pawn_GEN_OuvrirTrouee( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_OuvrirTrouee";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirTrouee destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_OuvrirTrouee::~Mission_Pawn_GEN_OuvrirTrouee()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirTrouee::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_OuvrirTrouee::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_OuvrirTrouee& asnMission = *new ASN1T_Mission_Pion_GEN_OuvrirTrouee();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ouvrir_trouee;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ouvrir_trouee = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_OuvrirTrouee::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_OuvrirTrouee::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_ouvrir_trouee );
    ASN1T_Mission_Pion_GEN_OuvrirTrouee& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ouvrir_trouee;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


