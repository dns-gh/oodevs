// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_AttaquerFormationAeromobile.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AttaquerFormationAeromobile constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_AttaquerFormationAeromobile::Mission_Automat_ALAT_AttaquerFormationAeromobile( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_AttaquerFormationAeromobile", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AttaquerFormationAeromobile destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_AttaquerFormationAeromobile::~Mission_Automat_ALAT_AttaquerFormationAeromobile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AttaquerFormationAeromobile::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_AttaquerFormationAeromobile::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *new ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_attaquer_formation_aeromobile = &asnMission;

    const T_IdVector& unitesAAttaquer_ = pTarget_->GetTestParam_AgentKnowledgeList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyAgentKnowledgeList( unitesAAttaquer_, asnMission.unites_a_attaquer );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );

    delete &unitesAAttaquer_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AttaquerFormationAeromobile::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_AttaquerFormationAeromobile::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_attaquer_formation_aeromobile );
    ASN1T_Mission_Automate_ALAT_AttaquerFormationAeromobile& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_attaquer_formation_aeromobile;

    ASN_Tools::Delete( asnMission.unites_a_attaquer );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




