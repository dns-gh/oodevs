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
#include "Mission_Automat_Test_MoveTo.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Test_MoveTo constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Test_MoveTo::Mission_Automat_Test_MoveTo( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_Test_MoveTo";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Test_MoveTo destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Test_MoveTo::~Mission_Automat_Test_MoveTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Test_MoveTo::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Test_MoveTo::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *new ASN1T_Mission_Automate_Test_MoveTo();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_test_move_to;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_test_move_to = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Test_MoveTo::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Test_MoveTo::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_test_move_to );
    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_test_move_to;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




