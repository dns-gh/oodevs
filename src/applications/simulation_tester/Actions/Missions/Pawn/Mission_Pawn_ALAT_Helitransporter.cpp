// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ALAT_Helitransporter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_Helitransporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_Helitransporter::Mission_Pawn_ALAT_Helitransporter( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_Helitransporter", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_Helitransporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_Helitransporter::~Mission_Pawn_ALAT_Helitransporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_Helitransporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_Helitransporter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ALAT_Helitransporter& asnMission = *new ASN1T_Mission_Pion_ALAT_Helitransporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_helitransporter;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_helitransporter = &asnMission;

    const T_IdVector& unitesAHelitransporter_ = pTarget_->GetTestParam_AgentList();
    const Position& pointDebarquement_ = pTarget_->GetTestParam_Point();
    const Position& pointEmbarquement_ = pTarget_->GetTestParam_Point();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyAgentList( unitesAHelitransporter_, asnMission.unites_a_helitransporter );
    ASN_Tools::CopyPoint( pointDebarquement_, asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pointEmbarquement_, asnMission.point_embarquement );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avec_materiel );

    delete &unitesAHelitransporter_;
    delete &pointDebarquement_;
    delete &pointEmbarquement_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_Helitransporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_Helitransporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_helitransporter );
    ASN1T_Mission_Pion_ALAT_Helitransporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_helitransporter;

    ASN_Tools::Delete( asnMission.unites_a_helitransporter );
    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


