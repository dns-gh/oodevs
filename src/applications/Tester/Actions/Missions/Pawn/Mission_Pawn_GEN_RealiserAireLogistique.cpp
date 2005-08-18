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
#include "Mission_Pawn_GEN_RealiserAireLogistique.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserAireLogistique constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserAireLogistique::Mission_Pawn_GEN_RealiserAireLogistique( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserAireLogistique";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserAireLogistique destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserAireLogistique::~Mission_Pawn_GEN_RealiserAireLogistique()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserAireLogistique::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserAireLogistique::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserAireLogistique();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_aire_logistique;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_aire_logistique = &asnMission;

    const Position& positionObstacle_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionObstacle_, asnMission.position_obstacle );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_obstacle_planifie );

    delete &positionObstacle_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserAireLogistique::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserAireLogistique::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_aire_logistique );
    ASN1T_Mission_Pion_GEN_RealiserAireLogistique& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_aire_logistique;

    ASN_Tools::Delete( asnMission.position_obstacle );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


