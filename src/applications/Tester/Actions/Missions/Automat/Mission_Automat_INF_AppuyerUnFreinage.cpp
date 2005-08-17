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
#include "Mission_Automat_INF_AppuyerUnFreinage.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUnFreinage::Mission_Automat_INF_AppuyerUnFreinage( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_AppuyerUnFreinage";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AppuyerUnFreinage::~Mission_Automat_INF_AppuyerUnFreinage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUnFreinage::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *new ASN1T_Mission_Automate_INF_AppuyerUnFreinage();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_appuyer_un_freinage;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer_un_freinage = &asnMission;

    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AppuyerUnFreinage::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AppuyerUnFreinage::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_appuyer_un_freinage );
    ASN1T_Mission_Automate_INF_AppuyerUnFreinage& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_appuyer_un_freinage;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




