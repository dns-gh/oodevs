// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASY_CommettreExactionsSurPopulationDansZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::~Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ASY_CommettreExactionsSurPopulationDansZone& asnMission = *new Mission_Automate_ASY_CommettreExactionsSurPopulationDansZone();

    asnMsg_.GetAsnMsg().mission().mission_automate_asy_commettre_exactions_sur_population_dans_zone = &asnMission;

    const T_LocationVector& zones_ = pTarget_->GetTestParam_PolygonList();

    ASN_Tools::CopyPolygonList( zones_, asnMission.zones );

    delete &zones_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASY_CommettreExactionsSurPopulationDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_asy_commettre_exactions_sur_population_dans_zone ());
    Mission_Automate_ASY_CommettreExactionsSurPopulationDansZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_asy_commettre_exactions_sur_population_dans_zone;

    ASN_Tools::Delete( asnMission.zones );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




