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
#include "Mission_Pawn_Test_Fire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Fire::Mission_Pawn_Test_Fire( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_Test_Fire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Test_Fire::~Mission_Pawn_Test_Fire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Fire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Test_Fire& asnMission = *new ASN1T_Mission_Pion_Test_Fire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_fire;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.tir_indirect );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 5 ), asnMission.munitions );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.nb_obus );
    ASN_Tools::CopyAgentKnowledgeList( pTarget_->GetTestParam_AgentKnowledgeList(), asnMission.targets );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Test_Fire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Test_Fire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_test_fire );
    ASN1T_Mission_Pion_Test_Fire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_test_fire;

    ASN_Tools::Delete( asnMission.point );
    ASN_Tools::Delete( asnMission.targets );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


