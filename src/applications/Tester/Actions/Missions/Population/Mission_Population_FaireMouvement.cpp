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
#include "Mission_Population_FaireMouvement.h"
#include "Entities/Population.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Population_FaireMouvement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_FaireMouvement::Mission_Population_FaireMouvement( Population& Population )
    : Mission_Population_ABC ( Population )
{
    strName_ = "Population_FaireMouvement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_FaireMouvement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Population_FaireMouvement::~Mission_Population_FaireMouvement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_FaireMouvement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Population_FaireMouvement::Serialize()
{
    // build din/asn msg
    Mission_Population_ABC::Serialize();

    ASN1T_Mission_Population_FaireMouvement& asnMission = *new ASN1T_Mission_Population_FaireMouvement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Population_mission_population_faire_mouvement;
    asnMsg_.GetAsnMsg().mission.u.mission_population_faire_mouvement = &asnMission;

    const Position& destination_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( destination_, asnMission.destination );

    delete &destination_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Population_FaireMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Population_FaireMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Population_mission_population_faire_mouvement );
    ASN1T_Mission_Population_FaireMouvement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_population_faire_mouvement;

    ASN_Tools::Delete( asnMission.destination );

    delete &asnMission;
    Mission_Population_ABC::Clean();
}




