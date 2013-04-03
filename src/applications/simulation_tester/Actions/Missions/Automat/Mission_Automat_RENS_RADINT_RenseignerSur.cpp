// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_RENS_RADINT_RenseignerSur.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_RADINT_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_RADINT_RenseignerSur::Mission_Automat_RENS_RADINT_RenseignerSur( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_RENS_RADINT_RenseignerSur", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_RADINT_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_RADINT_RenseignerSur::~Mission_Automat_RENS_RADINT_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_RADINT_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_RADINT_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *new Mission_Automate_RENS_RADINT_RenseignerSur();

    asnMsg_.GetAsnMsg().mission().mission_automate_rens_radint_renseignersur = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_RADINT_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_RADINT_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_rens_radint_renseignersur ());
    Mission_Automate_RENS_RADINT_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_rens_radint_renseignersur;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



