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
#include "Mission_Automat_GEN_RealiserCampRefugies.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampRefugies constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserCampRefugies::Mission_Automat_GEN_RealiserCampRefugies( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_RealiserCampRefugies";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampRefugies destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserCampRefugies::~Mission_Automat_GEN_RealiserCampRefugies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampRefugies::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserCampRefugies::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampRefugies();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_camp_refugies;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_refugies = &asnMission;

    const Location& camp_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( camp_, asnMission.camp );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.tc2 );

    delete &camp_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampRefugies::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserCampRefugies::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_camp_refugies );
    ASN1T_Mission_Automate_GEN_RealiserCampRefugies& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_refugies;

    ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




