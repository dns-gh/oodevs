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
#include "Mission_Automat_GEN_AppuyerDebarquementTroupes.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AppuyerDebarquementTroupes constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AppuyerDebarquementTroupes::Mission_Automat_GEN_AppuyerDebarquementTroupes( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_AppuyerDebarquementTroupes";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AppuyerDebarquementTroupes destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AppuyerDebarquementTroupes::~Mission_Automat_GEN_AppuyerDebarquementTroupes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AppuyerDebarquementTroupes::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AppuyerDebarquementTroupes::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *new ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_appuyer_debarquement_troupes = &asnMission;

    ASN_Tools::CopyGenObjectList( pTarget_->GetTestParam_GenObjectList(), asnMission.zones_debarquement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AppuyerDebarquementTroupes::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AppuyerDebarquementTroupes::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_appuyer_debarquement_troupes );
    ASN1T_Mission_Automate_GEN_AppuyerDebarquementTroupes& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_appuyer_debarquement_troupes;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




