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
#include "Mission_Automat_INF_Appuyer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Appuyer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Appuyer::Mission_Automat_INF_Appuyer( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_Appuyer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Appuyer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Appuyer::~Mission_Automat_INF_Appuyer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Appuyer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Appuyer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Appuyer& asnMission = *new ASN1T_Mission_Automate_INF_Appuyer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer = &asnMission;

    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Appuyer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Appuyer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_appuyer );
    ASN1T_Mission_Automate_INF_Appuyer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




