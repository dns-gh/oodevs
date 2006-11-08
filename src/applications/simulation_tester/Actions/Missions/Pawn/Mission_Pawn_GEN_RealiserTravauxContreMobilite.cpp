// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserTravauxContreMobilite.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxContreMobilite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxContreMobilite::Mission_Pawn_GEN_RealiserTravauxContreMobilite( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserTravauxContreMobilite", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxContreMobilite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxContreMobilite::~Mission_Pawn_GEN_RealiserTravauxContreMobilite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxContreMobilite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxContreMobilite::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserTravauxContreMobilite& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxContreMobilite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_contre_mobilite;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_contre_mobilite = &asnMission;

    const T_GenObjectVector& travaux_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( travaux_, asnMission.travaux );

    delete &travaux_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxContreMobilite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxContreMobilite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_contre_mobilite );
    ASN1T_Mission_Pion_GEN_RealiserTravauxContreMobilite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_contre_mobilite;

    ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


