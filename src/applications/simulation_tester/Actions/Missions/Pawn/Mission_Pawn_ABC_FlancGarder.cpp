// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_FlancGarder.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_FlancGarder constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_FlancGarder::Mission_Pawn_ABC_FlancGarder( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_FlancGarder", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_FlancGarder destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_FlancGarder::~Mission_Pawn_ABC_FlancGarder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_FlancGarder::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_FlancGarder::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ABC_FlancGarder& asnMission = *new Mission_Pion_ABC_FlancGarder();

    asnMsg_.GetAsnMsg().mission().mission_pion_abc_flanc_garder = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const T_IdVector& unitesACouvrir_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyAgentList( unitesACouvrir_, asnMission.unites_a_couvrir );

    delete &itineraire_;
    delete &unitesACouvrir_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_FlancGarder::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_FlancGarder::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_abc_flanc_garder ());
    Mission_Pion_ABC_FlancGarder& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_abc_flanc_garder;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.unites_a_couvrir );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


