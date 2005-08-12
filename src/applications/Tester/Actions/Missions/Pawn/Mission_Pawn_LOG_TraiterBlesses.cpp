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
#include "Mission_Pawn_LOG_TraiterBlesses.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TraiterBlesses constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TraiterBlesses::Mission_Pawn_LOG_TraiterBlesses( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_TraiterBlesses";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TraiterBlesses destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TraiterBlesses::~Mission_Pawn_LOG_TraiterBlesses()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TraiterBlesses::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TraiterBlesses::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_TraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TraiterBlesses();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_traiter_blesses;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_traiter_blesses = &asnMission;


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TraiterBlesses::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TraiterBlesses::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_traiter_blesses );
    ASN1T_Mission_Pion_LOG_TraiterBlesses& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_traiter_blesses;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


