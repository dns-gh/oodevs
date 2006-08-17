// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_EffectuerRechercheEtSauvetage", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::~Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage = &asnMission;

    const T_IdVector& unitesASecourir_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyAgentList( unitesASecourir_, asnMission.unites_a_secourir );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );

    delete &unitesASecourir_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_effectuer_recherche_et_sauvetage );
    ASN1T_Mission_Pion_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_effectuer_recherche_et_sauvetage;

    ASN_Tools::Delete( asnMission.unites_a_secourir );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


