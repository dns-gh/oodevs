// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ALAT_DetruireAeromobile.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireAeromobile constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_DetruireAeromobile::Mission_Pawn_ALAT_DetruireAeromobile( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_DetruireAeromobile", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireAeromobile destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_DetruireAeromobile::~Mission_Pawn_ALAT_DetruireAeromobile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireAeromobile::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_DetruireAeromobile::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ALAT_DetruireAeromobile& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireAeromobile();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_aeromobile;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_detruire_aeromobile = &asnMission;

    const T_IdVector& uniteADetruire_ = pTarget_->GetTestParam_AgentKnowledgeList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyAgentKnowledgeList( uniteADetruire_, asnMission.unite_a_detruire );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );

    delete &uniteADetruire_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireAeromobile::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_DetruireAeromobile::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_detruire_aeromobile );
    ASN1T_Mission_Pion_ALAT_DetruireAeromobile& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_detruire_aeromobile;

    ASN_Tools::Delete( asnMission.unite_a_detruire );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


