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
#include "Mission_Pawn_LOG_Livrer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Livrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Livrer::Mission_Pawn_LOG_Livrer( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_Livrer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Livrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_Livrer::~Mission_Pawn_LOG_Livrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Livrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Livrer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_Livrer& asnMission = *new ASN1T_Mission_Pion_LOG_Livrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_livrer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_livrer = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_Livrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_Livrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_livrer );
    ASN1T_Mission_Pion_LOG_Livrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_livrer;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


