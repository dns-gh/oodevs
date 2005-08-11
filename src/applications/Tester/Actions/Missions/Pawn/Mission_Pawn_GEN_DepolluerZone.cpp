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
#include "Mission_Pawn_GEN_DepolluerZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DepolluerZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DepolluerZone::Mission_Pawn_GEN_DepolluerZone( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_GEN_DepolluerZone";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DepolluerZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_DepolluerZone::~Mission_Pawn_GEN_DepolluerZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DepolluerZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DepolluerZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_DepolluerZone& asnMission = *new ASN1T_Mission_Pion_GEN_DepolluerZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_depolluer_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_depolluer_zone = &asnMission;

    ASN_Tools::CopyLocation( pTarget_->GetTestParam_Location(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_DepolluerZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_DepolluerZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_depolluer_zone );
    ASN1T_Mission_Pion_GEN_DepolluerZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_depolluer_zone;

    ASN_Tools::Delete( asnMission.zone );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


