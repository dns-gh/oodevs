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
#include "Mission_Pawn_GEN_NeutraliserEnginsExplosifs.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_NeutraliserEnginsExplosifs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_NeutraliserEnginsExplosifs::Mission_Pawn_GEN_NeutraliserEnginsExplosifs( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_NeutraliserEnginsExplosifs";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_NeutraliserEnginsExplosifs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_NeutraliserEnginsExplosifs::~Mission_Pawn_GEN_NeutraliserEnginsExplosifs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_NeutraliserEnginsExplosifs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_NeutraliserEnginsExplosifs::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *new ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_neutraliser_engins_explosifs = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_NeutraliserEnginsExplosifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_NeutraliserEnginsExplosifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_neutraliser_engins_explosifs );
    ASN1T_Mission_Pion_GEN_NeutraliserEnginsExplosifs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_neutraliser_engins_explosifs;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


