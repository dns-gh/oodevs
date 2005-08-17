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
#include "Mission_Automat_TRANS_Commander.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_TRANS_Commander constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_TRANS_Commander::Mission_Automat_TRANS_Commander( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_TRANS_Commander";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_TRANS_Commander destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_TRANS_Commander::~Mission_Automat_TRANS_Commander()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_TRANS_Commander::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_TRANS_Commander::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_TRANS_Commander& asnMission = *new ASN1T_Mission_Automate_TRANS_Commander();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_trans_commander;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_trans_commander = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_TRANS_Commander::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_TRANS_Commander::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_trans_commander );
    ASN1T_Mission_Automate_TRANS_Commander& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_trans_commander;

    ASN_Tools::Delete( asnMission.zone_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




