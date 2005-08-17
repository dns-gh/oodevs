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
#include "Mission_Automat_ALAT_EffectuerRechercheEtSauvetage.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_EffectuerRechercheEtSauvetage constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::Mission_Automat_ALAT_EffectuerRechercheEtSauvetage( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ALAT_EffectuerRechercheEtSauvetage";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_EffectuerRechercheEtSauvetage destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::~Mission_Automat_ALAT_EffectuerRechercheEtSauvetage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *new ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites_a_secourir );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_EffectuerRechercheEtSauvetage::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_effectuer_recherche_et_sauvetage );
    ASN1T_Mission_Automate_ALAT_EffectuerRechercheEtSauvetage& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_effectuer_recherche_et_sauvetage;

    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




