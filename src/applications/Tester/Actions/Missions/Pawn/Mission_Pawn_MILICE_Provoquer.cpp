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
#include "Mission_Pawn_MILICE_Provoquer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_Provoquer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_Provoquer::Mission_Pawn_MILICE_Provoquer( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_MILICE_Provoquer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_Provoquer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_Provoquer::~Mission_Pawn_MILICE_Provoquer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_Provoquer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_Provoquer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_MILICE_Provoquer& asnMission = *new ASN1T_Mission_Pion_MILICE_Provoquer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_milice_provoquer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_milice_provoquer = &asnMission;

    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.unite_a_provoquer );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_Provoquer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_Provoquer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_milice_provoquer );
    ASN1T_Mission_Pion_MILICE_Provoquer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_milice_provoquer;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


