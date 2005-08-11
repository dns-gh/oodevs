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
#include "Mission_Pawn_GEN_AppuyerDebarquementTroupesGU.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AppuyerDebarquementTroupesGU constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::Mission_Pawn_GEN_AppuyerDebarquementTroupesGU( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_AppuyerDebarquementTroupesGU";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AppuyerDebarquementTroupesGU destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::~Mission_Pawn_GEN_AppuyerDebarquementTroupesGU()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *new ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_appui );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_AppuyerDebarquementTroupesGU::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_appuyer_debarquement_troupes_gu );
    ASN1T_Mission_Pion_GEN_AppuyerDebarquementTroupesGU& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_appuyer_debarquement_troupes_gu;

    ASN_Tools::Delete( asnMission.zone_appui );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


