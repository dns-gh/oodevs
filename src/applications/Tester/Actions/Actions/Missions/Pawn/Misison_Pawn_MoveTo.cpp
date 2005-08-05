// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
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
#include "Mission_Pawn_MoveTo.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MoveTo constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MoveTo::Mission_Pawn_MoveTo( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Action< Pawn > ( pawn, nExecutionTick )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MoveTo destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MoveTo::~Mission_Pawn_MoveTo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MoveTo::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
bool Mission_Pawn_MoveTo::Serialize() const
{
    // build din/asn msg
    MOS_ASN_MsgPionOrder      asnMsg;
    ASN_Tools::CopyId         ( pTarget_->GetId()                             , asnMsg.GetAsnMsg().oid_unite_executante );
    ASN_Tools::CopyId         ( IDManager::orderIDManager_.GetFreeIdentifier(), asnMsg.GetAsnMsg().order_id             );
    ASN_Tools::CopyId         ( pTarget_->GetTP_LeftLimit()                   , asnMsg.GetAsnMsg().oid_limite_gauche    );
    ASN_Tools::CopyId         ( pTarget_->GetTP_RightLimit()                  , asnMsg.GetAsnMsg().oid_limite_droite    );
    ASN_Tools::CopyIdList     ( pTarget_->GetTP_Limas()                       , asnMsg.GetAsnMsg().oid_limas            );
    ASN_Tools::CopyDirection  ( pTarget_->GetTP_DangerDirection()             , asnMsg.GetAsnMsg().direction_dangereuse );

    ASN1T_Mission_Pion_Test_MoveTo& asnMission             = *new ASN1T_Mission_Pion_Test_MoveTo();
    asnMsg.GetAsnMsg().mission.t                           = T_Mission_Pion_mission_pion_test_move_to;
    asnMsg.GetAsnMsg().mission.u.mission_pion_test_move_to = &asnMission;

    ASN_Tools::CopyPath       ( pTarget_->GetTP_Path()             , asnMission.itineraire                   );
    ASN_Tools::CopyEnumeration( pTarget_->GetTP_PathType()         , ( uint& )asnMission.type_itineraire     );
    ASN_Tools::CopyBool       ( pTarget_->GetTP_UnLoaded()         , asnMission.debarque                     );
    ASN_Tools::CopyEnumeration( pTarget_->GetTP_VerouillageVision(), ( uint& )asnMission.verrouillage_vision );
    ASN_Tools::CopyPosition   ( pTarget_->GetTP_VisionPoint()      , asnMission.vision_point                 );
    ASN_Tools::CopyDirection  ( pTarget_->GetTP_Direction()        , asnMission.vision_direction             );

    // send to SIM
    asnMsg.Send( 56 );

    // clean
    std::cout << "mission pawn moveTo" << std::endl;
    return true;
}
