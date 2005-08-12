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
#include "Mission_Pawn_LOG_ArmerPIA.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ArmerPIA::Mission_Pawn_LOG_ArmerPIA( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_ArmerPIA";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_ArmerPIA::~Mission_Pawn_LOG_ArmerPIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ArmerPIA::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *new ASN1T_Mission_Pion_LOG_ArmerPIA();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_armer_pia;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_armer_pia = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_ArmerPIA::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_ArmerPIA::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_armer_pia );
    ASN1T_Mission_Pion_LOG_ArmerPIA& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_armer_pia;

    ASN_Tools::Delete( asnMission.point );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


