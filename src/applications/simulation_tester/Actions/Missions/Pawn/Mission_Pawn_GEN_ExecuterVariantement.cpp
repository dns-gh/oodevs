// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_ExecuterVariantement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ExecuterVariantement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ExecuterVariantement::Mission_Pawn_GEN_ExecuterVariantement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_ExecuterVariantement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ExecuterVariantement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_ExecuterVariantement::~Mission_Pawn_GEN_ExecuterVariantement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ExecuterVariantement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ExecuterVariantement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *new ASN1T_Mission_Pion_GEN_ExecuterVariantement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_executer_variantement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_executer_variantement = &asnMission;

    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_franchissement );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_objet_planifie );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.type_pontage );

    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_ExecuterVariantement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_ExecuterVariantement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_executer_variantement );
    ASN1T_Mission_Pion_GEN_ExecuterVariantement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_executer_variantement;

    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


