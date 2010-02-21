// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Defendre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Defendre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Defendre::Mission_Pawn_INF_Defendre( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Defendre", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Defendre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Defendre::~Mission_Pawn_INF_Defendre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Defendre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Defendre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Defendre& asnMission = *new Mission_Pion_INF_Defendre();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_defendre = &asnMission;

    const Position& pointADefendre_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointADefendre_, asnMission.point_a_defendre );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &pointADefendre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Defendre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Defendre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_defendre ());
    Mission_Pion_INF_Defendre& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_defendre;

    ASN_Tools::Delete( asnMission.point_a_defendre );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


