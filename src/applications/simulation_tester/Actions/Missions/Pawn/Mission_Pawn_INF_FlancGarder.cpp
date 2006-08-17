// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_INF_FlancGarder.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_FlancGarder constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_FlancGarder::Mission_Pawn_INF_FlancGarder( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_FlancGarder", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_FlancGarder destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_FlancGarder::~Mission_Pawn_INF_FlancGarder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_FlancGarder::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_FlancGarder::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *new ASN1T_Mission_Pion_INF_FlancGarder();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_flanc_garder;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_flanc_garder = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_flancgarder );

    delete &itineraire_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_FlancGarder::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_FlancGarder::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_flanc_garder );
    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_flanc_garder;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


