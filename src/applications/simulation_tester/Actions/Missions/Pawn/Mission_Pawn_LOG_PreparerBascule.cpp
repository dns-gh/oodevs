// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_LOG_PreparerBascule.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PreparerBascule constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_PreparerBascule::Mission_Pawn_LOG_PreparerBascule( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_PreparerBascule", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PreparerBascule destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_PreparerBascule::~Mission_Pawn_LOG_PreparerBascule()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PreparerBascule::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_PreparerBascule::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *new ASN1T_Mission_Pion_LOG_PreparerBascule();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_preparer_bascule;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_preparer_bascule = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.automate_pour_bascule );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.rester_sur_place );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_PreparerBascule::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_PreparerBascule::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_preparer_bascule );
    ASN1T_Mission_Pion_LOG_PreparerBascule& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_preparer_bascule;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


