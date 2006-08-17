// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ABC_AppuyerSurPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_AppuyerSurPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_AppuyerSurPosition::Mission_Pawn_ABC_AppuyerSurPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_AppuyerSurPosition", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_AppuyerSurPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_AppuyerSurPosition::~Mission_Pawn_ABC_AppuyerSurPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_AppuyerSurPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_AppuyerSurPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Pion_ABC_AppuyerSurPosition();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_appuyer_sur_position;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_appuyer_sur_position = &asnMission;

    const Position& position_ = pTarget_->GetTestParam_Point();
    const T_IdVector& unitesAAppuyer_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyPoint( position_, asnMission.position );
    ASN_Tools::CopyAgentList( unitesAAppuyer_, asnMission.unites_a_appuyer );

    delete &position_;
    delete &unitesAAppuyer_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_AppuyerSurPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_AppuyerSurPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_appuyer_sur_position );
    ASN1T_Mission_Pion_ABC_AppuyerSurPosition& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_appuyer_sur_position;

    ASN_Tools::Delete( asnMission.position );
    ASN_Tools::Delete( asnMission.unites_a_appuyer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


