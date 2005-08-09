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
#include "Mission_Pawn_Fire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Fire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Fire::Mission_Pawn_Fire( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Fire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Fire::~Mission_Pawn_Fire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Fire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Fire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_Fire& asnMission             = *new ASN1T_Mission_Pion_Test_Fire();
    asnMsg_.GetAsnMsg().mission.t                        = T_Mission_Pion_mission_pion_test_fire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;

    ASN_Tools::CopyPosition          ( pTarget_->GetTP_Position()      , asnMission.point              );
    ASN_Tools::CopyBool              ( pTarget_->GetTP_IsIndirectFire(), asnMission.tir_indirect       );
    ASN_Tools::CopyEnumeration       ( pTarget_->GetTP_MunitionType()  , ( uint& )asnMission.munitions );
    ASN_Tools::CopyInteger           ( pTarget_->GetTP_NbObus()        , asnMission.nb_obus            );
    ASN_Tools::CopyAgentKnowledgeList( pTarget_->GetTP_Targets()       , asnMission.targets            );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Fire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Fire::Clean()
{
    //delete &asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire;
    Mission_Pawn_ABC::Clean();
}
