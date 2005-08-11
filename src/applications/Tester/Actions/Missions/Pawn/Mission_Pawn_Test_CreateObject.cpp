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
#include "Mission_Pawn_Test_CreateObject.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateObject constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_CreateObject::Mission_Pawn_Test_CreateObject( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_Test_CreateObject";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateObject destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_CreateObject::~Mission_Pawn_Test_CreateObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateObject::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_CreateObject::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *new ASN1T_Mission_Pion_Test_CreateObject();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_create_object;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_object = &asnMission;

    ASN_Tools::CopyLocation( pTarget_->GetTestParam_Location(), asnMission.forme );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 37 ), asnMission.type );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.param );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_CreateObject::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_CreateObject::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_create_object );
    ASN1T_Mission_Pion_Test_CreateObject& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_object;

    ASN_Tools::Delete( asnMission.forme );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


