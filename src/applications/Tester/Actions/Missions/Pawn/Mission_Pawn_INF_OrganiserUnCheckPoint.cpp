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
#include "Mission_Pawn_INF_OrganiserUnCheckPoint.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_OrganiserUnCheckPoint constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_OrganiserUnCheckPoint::Mission_Pawn_INF_OrganiserUnCheckPoint( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_OrganiserUnCheckPoint";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_OrganiserUnCheckPoint destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_OrganiserUnCheckPoint::~Mission_Pawn_INF_OrganiserUnCheckPoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_OrganiserUnCheckPoint::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_OrganiserUnCheckPoint::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *new ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_organiser_un_check_point;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_organiser_un_check_point = &asnMission;

    const Position& pointInstallation_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointInstallation_, asnMission.point_installation );

    delete &pointInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_OrganiserUnCheckPoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_OrganiserUnCheckPoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_organiser_un_check_point );
    ASN1T_Mission_Pion_INF_OrganiserUnCheckPoint& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_organiser_un_check_point;

    ASN_Tools::Delete( asnMission.point_installation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


