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
#include "Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_AppuyerFranchissementSurUnPoint";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::~Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_franchissement_sur_un_point = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.pont_flottant );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_appuyer_franchissement_sur_un_point );
    ASN1T_Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_franchissement_sur_un_point;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


