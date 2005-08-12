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
#include "Mission_Pawn_GEN_ReprendreMissionConstruction.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreMissionConstruction constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReprendreMissionConstruction::Mission_Pawn_GEN_ReprendreMissionConstruction( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_ReprendreMissionConstruction";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreMissionConstruction destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ReprendreMissionConstruction::~Mission_Pawn_GEN_ReprendreMissionConstruction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreMissionConstruction::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReprendreMissionConstruction::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction& asnMission = *new ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_reprendre_mission_construction;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_reprendre_mission_construction = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.travaux );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ReprendreMissionConstruction::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ReprendreMissionConstruction::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_reprendre_mission_construction );
    ASN1T_Mission_Pion_GEN_ReprendreMissionConstruction& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_reprendre_mission_construction;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


