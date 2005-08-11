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
#include "Mission_Pawn_GEN_AmeliorerMobilite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AmeliorerMobilite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_AmeliorerMobilite::Mission_Pawn_GEN_AmeliorerMobilite( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_AmeliorerMobilite";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AmeliorerMobilite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_AmeliorerMobilite::~Mission_Pawn_GEN_AmeliorerMobilite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AmeliorerMobilite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_AmeliorerMobilite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_AmeliorerMobilite& asnMission = *new ASN1T_Mission_Pion_GEN_AmeliorerMobilite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_ameliorer_mobilite;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ameliorer_mobilite = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AmeliorerMobilite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_AmeliorerMobilite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_ameliorer_mobilite );
    ASN1T_Mission_Pion_GEN_AmeliorerMobilite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_ameliorer_mobilite;

    ASN_Tools::Delete( asnMission.zone );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


