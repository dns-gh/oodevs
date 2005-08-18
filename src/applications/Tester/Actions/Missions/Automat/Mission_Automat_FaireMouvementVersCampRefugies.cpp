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
#include "Mission_Automat_FaireMouvementVersCampRefugies.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvementVersCampRefugies::Mission_Automat_FaireMouvementVersCampRefugies( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_FaireMouvementVersCampRefugies";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FaireMouvementVersCampRefugies::~Mission_Automat_FaireMouvementVersCampRefugies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvementVersCampRefugies::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *new ASN1T_Mission_Automate_FaireMouvementVersCampRefugies();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_faire_mouvement_vers_camp_refugies = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FaireMouvementVersCampRefugies::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_FaireMouvementVersCampRefugies::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_faire_mouvement_vers_camp_refugies );
    ASN1T_Mission_Automate_FaireMouvementVersCampRefugies& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_faire_mouvement_vers_camp_refugies;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




