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
#include "Mission_Pawn_ASA_DefendreUnites.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreUnites::Mission_Pawn_ASA_DefendreUnites( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ASA_DefendreUnites";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreUnites::~Mission_Pawn_ASA_DefendreUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreUnites::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreUnites();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_unites;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_unites = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_defendre_unites );
    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_unites;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


