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
#include "Mission_Pawn_INF_Barrer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Barrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Barrer::Mission_Pawn_INF_Barrer( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_Barrer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Barrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Barrer::~Mission_Pawn_INF_Barrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Barrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Barrer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_Barrer& asnMission = *new ASN1T_Mission_Pion_INF_Barrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_barrer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_barrer = &asnMission;

    const Position& positionABarrer_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionABarrer_, asnMission.position_a_barrer );

    delete &positionABarrer_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Barrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Barrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_barrer );
    ASN1T_Mission_Pion_INF_Barrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_barrer;

    ASN_Tools::Delete( asnMission.position_a_barrer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


