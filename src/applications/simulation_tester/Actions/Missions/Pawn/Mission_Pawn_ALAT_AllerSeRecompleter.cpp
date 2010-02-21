// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_AllerSeRecompleter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AllerSeRecompleter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_AllerSeRecompleter::Mission_Pawn_ALAT_AllerSeRecompleter( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_AllerSeRecompleter", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AllerSeRecompleter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_AllerSeRecompleter::~Mission_Pawn_ALAT_AllerSeRecompleter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AllerSeRecompleter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_AllerSeRecompleter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *new Mission_Pion_ALAT_AllerSeRecompleter();

    asnMsg_.GetAsnMsg().mission().mission_pion_alat_aller_se_recompleter = &asnMission;

    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AllerSeRecompleter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_AllerSeRecompleter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_alat_aller_se_recompleter ());
    Mission_Pion_ALAT_AllerSeRecompleter& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_alat_aller_se_recompleter;

    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


