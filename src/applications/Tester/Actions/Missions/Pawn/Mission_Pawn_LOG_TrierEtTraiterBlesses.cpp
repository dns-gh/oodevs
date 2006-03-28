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
#include "Mission_Pawn_LOG_TrierEtTraiterBlesses.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TrierEtTraiterBlesses::Mission_Pawn_LOG_TrierEtTraiterBlesses( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_TrierEtTraiterBlesses";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_TrierEtTraiterBlesses::~Mission_Pawn_LOG_TrierEtTraiterBlesses()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TrierEtTraiterBlesses::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *new ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses = &asnMission;

    const & blessuresTraitees_ = pTarget_->GetTestParam_MedicalPriorities();

    ASN_Tools::CopyMedicalPriorities( blessuresTraitees_, asnMission.blessures_traitees );

    delete &blessuresTraitees_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_TrierEtTraiterBlesses::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_TrierEtTraiterBlesses::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_trier_et_traiter_blesses );
    ASN1T_Mission_Pion_LOG_TrierEtTraiterBlesses& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_trier_et_traiter_blesses;

    ASN_Tools::Delete( asnMission.blessures_traitees );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


