// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Pawn_ABC ( "Pawn_LOG_AppuyerFranchissementSurUnPoint", pawn )
{
    // NOTHING
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

    Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *new Mission_Pion_LOG_AppuyerFranchissementSurUnPoint();

    asnMsg_.GetAsnMsg().mission().mission_pion_log_appuyer_franchissement_sur_un_point = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.pont_flottant );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerFranchissementSurUnPoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_log_appuyer_franchissement_sur_un_point ());
    Mission_Pion_LOG_AppuyerFranchissementSurUnPoint& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_log_appuyer_franchissement_sur_un_point;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


