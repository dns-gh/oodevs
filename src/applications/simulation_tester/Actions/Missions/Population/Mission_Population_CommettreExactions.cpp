// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Population_CommettreExactions.h"
#include "Entities/Population.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Population_CommettreExactions constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_CommettreExactions::Mission_Population_CommettreExactions( Population& Population )
    : Mission_Population_ABC ( "Population_CommettreExactions", Population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_CommettreExactions destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_CommettreExactions::~Mission_Population_CommettreExactions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_CommettreExactions::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Population_CommettreExactions::Serialize()
{
    // build din/asn msg
    Mission_Population_ABC::Serialize();

    ASN1T_Mission_Population_CommettreExactions& asnMission = *new ASN1T_Mission_Population_CommettreExactions();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Population_mission_population_commettre_exactions;
    asnMsg_.GetAsnMsg().mission.u.mission_population_commettre_exactions = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Population_CommettreExactions::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_CommettreExactions::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Population_mission_population_commettre_exactions );
    ASN1T_Mission_Population_CommettreExactions& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_population_commettre_exactions;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Population_ABC::Clean();
}




