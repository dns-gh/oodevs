// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_GEN_OuvrirAxesProgression.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_OuvrirAxesProgression constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_OuvrirAxesProgression::Mission_Automat_GEN_OuvrirAxesProgression( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_OuvrirAxesProgression", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_OuvrirAxesProgression destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_OuvrirAxesProgression::~Mission_Automat_GEN_OuvrirAxesProgression()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_OuvrirAxesProgression::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_OuvrirAxesProgression::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *new ASN1T_Mission_Automate_GEN_OuvrirAxesProgression();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_ouvrir_axes_progression = &asnMission;

    const T_PathVector& axes_ = pTarget_->GetTestParam_PathList();

    ASN_Tools::CopyPathList( axes_, asnMission.axes );

    delete &axes_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_OuvrirAxesProgression::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_OuvrirAxesProgression::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_ouvrir_axes_progression );
    ASN1T_Mission_Automate_GEN_OuvrirAxesProgression& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_ouvrir_axes_progression;

    ASN_Tools::Delete( asnMission.axes );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




