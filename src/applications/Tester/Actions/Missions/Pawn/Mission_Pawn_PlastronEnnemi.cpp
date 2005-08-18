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
#include "Mission_Pawn_PlastronEnnemi.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_PlastronEnnemi::Mission_Pawn_PlastronEnnemi( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_PlastronEnnemi";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_PlastronEnnemi::~Mission_Pawn_PlastronEnnemi()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_PlastronEnnemi::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *new ASN1T_Mission_Pion_PlastronEnnemi();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_plastron_ennemi;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_plastron_ennemi = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointMission_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointMission_, asnMission.point_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.type );

    delete &itineraire_;
    delete &pointMission_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_PlastronEnnemi::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_PlastronEnnemi::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_plastron_ennemi );
    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_plastron_ennemi;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_mission );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


