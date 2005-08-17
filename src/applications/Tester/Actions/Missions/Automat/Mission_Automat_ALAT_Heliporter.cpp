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
#include "Mission_Automat_ALAT_Heliporter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Heliporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Heliporter::Mission_Automat_ALAT_Heliporter( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ALAT_Heliporter";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Heliporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Heliporter::~Mission_Automat_ALAT_Heliporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Heliporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Heliporter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_Heliporter& asnMission = *new ASN1T_Mission_Automate_ALAT_Heliporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_heliporter;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_heliporter = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites_a_heliporter );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_embarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.attendre_unites );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avec_materiel );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Heliporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Heliporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_heliporter );
    ASN1T_Mission_Automate_ALAT_Heliporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_heliporter;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




