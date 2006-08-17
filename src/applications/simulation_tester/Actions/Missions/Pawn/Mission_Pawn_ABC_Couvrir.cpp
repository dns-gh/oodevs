// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ABC_Couvrir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Couvrir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Couvrir::Mission_Pawn_ABC_Couvrir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_Couvrir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Couvrir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_Couvrir::~Mission_Pawn_ABC_Couvrir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Couvrir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Couvrir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *new ASN1T_Mission_Pion_ABC_Couvrir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_couvrir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_couvrir = &asnMission;

    const Position& position_ = pTarget_->GetTestParam_Point();
    const T_IdVector& unitesACouvrir_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyPoint( position_, asnMission.position );
    ASN_Tools::CopyAgentList( unitesACouvrir_, asnMission.unites_a_couvrir );

    delete &position_;
    delete &unitesACouvrir_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_Couvrir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_Couvrir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_couvrir );
    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_couvrir;

    ASN_Tools::Delete( asnMission.position );
    ASN_Tools::Delete( asnMission.unites_a_couvrir );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


