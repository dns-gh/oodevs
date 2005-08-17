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
#include "Mission_Automat_ALAT_ExtrairePersonnel.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_ExtrairePersonnel constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_ExtrairePersonnel::Mission_Automat_ALAT_ExtrairePersonnel( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ALAT_ExtrairePersonnel";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_ExtrairePersonnel destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_ExtrairePersonnel::~Mission_Automat_ALAT_ExtrairePersonnel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_ExtrairePersonnel::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_ExtrairePersonnel::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *new ASN1T_Mission_Automate_ALAT_ExtrairePersonnel();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_extraire_personnel;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_extraire_personnel = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites_a_helitransporter );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_embarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.avec_materiel );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_ExtrairePersonnel::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_ExtrairePersonnel::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_extraire_personnel );
    ASN1T_Mission_Automate_ALAT_ExtrairePersonnel& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_extraire_personnel;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.point_embarquement );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




