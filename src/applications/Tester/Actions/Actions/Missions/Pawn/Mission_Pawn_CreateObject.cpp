// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
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
#include "Mission_Pawn_CreateObject.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CreateObject constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_CreateObject::Mission_Pawn_CreateObject( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CreateObject destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_CreateObject::~Mission_Pawn_CreateObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CreateObject::Serialize
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_CreateObject::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_CreateObject& asnMission              = *new ASN1T_Mission_Pion_Test_CreateObject();
    asnMsg_.GetAsnMsg().mission.t                                 = T_Mission_Pion_mission_pion_test_create_object;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_object = &asnMission;

    ASN_Tools::CopyPosition   ( pTarget_->GetTP_Position()  , asnMission.forme         );
    ASN_Tools::CopyEnumeration( pTarget_->GetTP_ObjectType(), ( uint& )asnMission.type );
    ASN_Tools::CopyInteger    ( 0                           , asnMission.param         );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_CreateObject::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_CreateObject::Clean()
{
    //delete &asnMsg_.GetAsnMsg().mission.u.mission_pion_test_create_object;
    Mission_Pawn_ABC::Clean();
}