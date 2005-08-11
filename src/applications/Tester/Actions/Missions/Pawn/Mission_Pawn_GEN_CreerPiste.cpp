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
#include "Mission_Pawn_GEN_CreerPiste.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPiste constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerPiste::Mission_Pawn_GEN_CreerPiste( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_CreerPiste";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPiste destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_CreerPiste::~Mission_Pawn_GEN_CreerPiste()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPiste::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerPiste::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *new ASN1T_Mission_Pion_GEN_CreerPiste();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_creer_piste;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_piste = &asnMission;

    ASN_Tools::CopyLocation( pTarget_->GetTestParam_Location(), asnMission.pos_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_CreerPiste::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_CreerPiste::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_creer_piste );
    ASN1T_Mission_Pion_GEN_CreerPiste& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_creer_piste;

    ASN_Tools::Delete( asnMission.pos_obstacle );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


