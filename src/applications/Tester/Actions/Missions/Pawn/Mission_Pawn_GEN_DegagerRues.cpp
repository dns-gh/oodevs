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
#include "Mission_Pawn_GEN_DegagerRues.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerRues constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerRues::Mission_Pawn_GEN_DegagerRues( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_DegagerRues";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerRues destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DegagerRues::~Mission_Pawn_GEN_DegagerRues()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerRues::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerRues::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *new ASN1T_Mission_Pion_GEN_DegagerRues();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_degager_rues;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_rues = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DegagerRues::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DegagerRues::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_degager_rues );
    ASN1T_Mission_Pion_GEN_DegagerRues& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_degager_rues;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


