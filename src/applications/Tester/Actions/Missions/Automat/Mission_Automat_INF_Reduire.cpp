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
#include "Mission_Automat_INF_Reduire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Reduire::Mission_Automat_INF_Reduire( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_Reduire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Reduire::~Mission_Automat_INF_Reduire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Reduire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Reduire& asnMission = *new ASN1T_Mission_Automate_INF_Reduire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_reduire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_reduire = &asnMission;

    ASN_Tools::CopyPointList( pTarget_->GetTestParam_PointList(), asnMission.points_resistance );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Reduire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Reduire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_reduire );
    ASN1T_Mission_Automate_INF_Reduire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_reduire;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




