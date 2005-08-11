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
#include "Mission_Pawn_Stationner.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Stationner::Mission_Pawn_Stationner( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_Stationner";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Stationner::~Mission_Pawn_Stationner()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Stationner::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Stationner& asnMission = *new ASN1T_Mission_Pion_Stationner();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_stationner;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_stationner = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_attente );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Stationner::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Stationner::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_stationner );
    ASN1T_Mission_Pion_Stationner& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_stationner;

    ASN_Tools::Delete( asnMission.point_attente );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


