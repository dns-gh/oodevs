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
#include "Mission_Automat_GEN_AmenagerAireStationnement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerAireStationnement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AmenagerAireStationnement::Mission_Automat_GEN_AmenagerAireStationnement( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_AmenagerAireStationnement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerAireStationnement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AmenagerAireStationnement::~Mission_Automat_GEN_AmenagerAireStationnement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerAireStationnement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AmenagerAireStationnement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_AmenagerAireStationnement& asnMission = *new ASN1T_Mission_Automate_GEN_AmenagerAireStationnement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_amenager_aire_stationnement = &asnMission;

    ASN_Tools::CopyGenObjectList( pTarget_->GetTestParam_GenObjectList(), asnMission.travaux );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerAireStationnement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AmenagerAireStationnement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_amenager_aire_stationnement );
    ASN1T_Mission_Automate_GEN_AmenagerAireStationnement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_amenager_aire_stationnement;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




