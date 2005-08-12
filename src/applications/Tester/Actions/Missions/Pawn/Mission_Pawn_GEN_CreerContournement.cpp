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
#include "Mission_Pawn_GEN_CreerContournement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerContournement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerContournement::Mission_Pawn_GEN_CreerContournement( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_CreerContournement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerContournement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerContournement::~Mission_Pawn_GEN_CreerContournement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerContournement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerContournement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_CreerContournement& asnMission = *new ASN1T_Mission_Pion_GEN_CreerContournement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_contournement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_contournement = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.obstacle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerContournement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerContournement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_creer_contournement );
    ASN1T_Mission_Pion_GEN_CreerContournement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_contournement;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


