// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

    Mission_Automate_ALAT_AllerSeRecompleter& asnMission = *new Mission_Automate_ALAT_AllerSeRecompleter();

    asnMsg_.GetAsnMsg().mission().mission_automate_alat_aller_se_recompleter = &asnMission;

    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_AllerSeRecompleter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_AllerSeRecompleter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_alat_aller_se_recompleter ());
    Mission_Automate_ALAT_AllerSeRecompleter& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_alat_aller_se_recompleter;

    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



