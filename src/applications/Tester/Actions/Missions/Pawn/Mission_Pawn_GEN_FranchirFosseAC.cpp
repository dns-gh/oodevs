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
#include "Mission_Pawn_GEN_FranchirFosseAC.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_FranchirFosseAC constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_FranchirFosseAC::Mission_Pawn_GEN_FranchirFosseAC( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_FranchirFosseAC";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_FranchirFosseAC destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_FranchirFosseAC::~Mission_Pawn_GEN_FranchirFosseAC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_FranchirFosseAC::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_FranchirFosseAC::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_FranchirFosseAC& asnMission = *new ASN1T_Mission_Pion_GEN_FranchirFosseAC();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_franchir_fosse_ac;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_franchir_fosse_ac = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_FranchirFosseAC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_FranchirFosseAC::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_franchir_fosse_ac );
    ASN1T_Mission_Pion_GEN_FranchirFosseAC& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_franchir_fosse_ac;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


