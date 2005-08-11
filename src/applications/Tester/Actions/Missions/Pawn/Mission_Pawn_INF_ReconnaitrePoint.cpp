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
#include "Mission_Pawn_INF_ReconnaitrePoint.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ReconnaitrePoint constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_ReconnaitrePoint::Mission_Pawn_INF_ReconnaitrePoint( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_INF_ReconnaitrePoint";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ReconnaitrePoint destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_ReconnaitrePoint::~Mission_Pawn_INF_ReconnaitrePoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ReconnaitrePoint::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_ReconnaitrePoint::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Pion_INF_ReconnaitrePoint();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_reconnaitre_point;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_point = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_a_reconnaitre );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ReconnaitrePoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_ReconnaitrePoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_reconnaitre_point );
    ASN1T_Mission_Pion_INF_ReconnaitrePoint& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_reconnaitre_point;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_a_reconnaitre );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


