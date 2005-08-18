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
#include "Mission_Pawn_INF_Couvrir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Couvrir::Mission_Pawn_INF_Couvrir( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_Couvrir";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Couvrir::~Mission_Pawn_INF_Couvrir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Couvrir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *new ASN1T_Mission_Pion_INF_Couvrir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_couvrir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_couvrir = &asnMission;

    const Position& pointCouverture_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointCouverture_, asnMission.point_couverture );
    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.cible );

    delete &pointCouverture_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Couvrir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Couvrir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_couvrir );
    ASN1T_Mission_Pion_INF_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_couvrir;

    ASN_Tools::Delete( asnMission.point_couverture );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


