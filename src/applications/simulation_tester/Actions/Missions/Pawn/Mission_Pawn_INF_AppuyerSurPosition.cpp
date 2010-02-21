// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_AppuyerSurPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_AppuyerSurPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_AppuyerSurPosition::Mission_Pawn_INF_AppuyerSurPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_AppuyerSurPosition", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_AppuyerSurPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_AppuyerSurPosition::~Mission_Pawn_INF_AppuyerSurPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_AppuyerSurPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_AppuyerSurPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_AppuyerSurPosition& asnMission = *new Mission_Pion_INF_AppuyerSurPosition();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_appuyer_sur_position = &asnMission;

    const Position& position_ = pTarget_->GetTestParam_Point();
    const T_IdVector& unitesAAppuyer_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyPoint( position_, asnMission.position );
    ASN_Tools::CopyAgentList( unitesAAppuyer_, asnMission.unites_a_appuyer );

    delete &position_;
    delete &unitesAAppuyer_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_AppuyerSurPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_AppuyerSurPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_appuyer_sur_position ());
    Mission_Pion_INF_AppuyerSurPosition& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_appuyer_sur_position;

    ASN_Tools::Delete( asnMission.position );
    ASN_Tools::Delete( asnMission.unites_a_appuyer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


