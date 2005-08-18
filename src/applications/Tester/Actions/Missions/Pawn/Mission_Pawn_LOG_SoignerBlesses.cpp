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
#include "Mission_Pawn_LOG_SoignerBlesses.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_SoignerBlesses constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_SoignerBlesses::Mission_Pawn_LOG_SoignerBlesses( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_SoignerBlesses";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_SoignerBlesses destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_SoignerBlesses::~Mission_Pawn_LOG_SoignerBlesses()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_SoignerBlesses::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_SoignerBlesses::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_SoignerBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_SoignerBlesses();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_soigner_blesses;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_soigner_blesses = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_SoignerBlesses::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_SoignerBlesses::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_soigner_blesses );
    ASN1T_Mission_Pion_LOG_SoignerBlesses& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_soigner_blesses;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


