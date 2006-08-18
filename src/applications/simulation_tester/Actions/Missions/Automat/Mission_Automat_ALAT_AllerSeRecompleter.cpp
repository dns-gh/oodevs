// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_AllerSeRecompleter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AllerSeRecompleter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_AllerSeRecompleter::Mission_Automat_ALAT_AllerSeRecompleter( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_AllerSeRecompleter", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AllerSeRecompleter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_AllerSeRecompleter::~Mission_Automat_ALAT_AllerSeRecompleter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AllerSeRecompleter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_AllerSeRecompleter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_AllerSeRecompleter& asnMission = *new ASN1T_Mission_Automate_ALAT_AllerSeRecompleter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_aller_se_recompleter;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_aller_se_recompleter = &asnMission;

    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );

    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AllerSeRecompleter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_AllerSeRecompleter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_aller_se_recompleter );
    ASN1T_Mission_Automate_ALAT_AllerSeRecompleter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_aller_se_recompleter;

    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




