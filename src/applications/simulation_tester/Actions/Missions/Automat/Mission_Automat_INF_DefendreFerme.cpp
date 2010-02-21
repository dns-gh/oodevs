// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_DefendreFerme.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_DefendreFerme constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_DefendreFerme::Mission_Automat_INF_DefendreFerme( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_DefendreFerme", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_DefendreFerme destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_DefendreFerme::~Mission_Automat_INF_DefendreFerme()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_DefendreFerme::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_DefendreFerme::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_DefendreFerme& asnMission = *new Mission_Automate_INF_DefendreFerme();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_defendre_ferme = &asnMission;

    const Location& position_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( position_, asnMission.position );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &position_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_DefendreFerme::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_DefendreFerme::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_defendre_ferme ());
    Mission_Automate_INF_DefendreFerme& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_defendre_ferme;

    ASN_Tools::Delete( asnMission.position );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




