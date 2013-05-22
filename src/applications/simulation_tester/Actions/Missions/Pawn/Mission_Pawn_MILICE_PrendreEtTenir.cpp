// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_MILICE_PrendreEtTenir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_PrendreEtTenir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_PrendreEtTenir::Mission_Pawn_MILICE_PrendreEtTenir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_MILICE_PrendreEtTenir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_PrendreEtTenir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_PrendreEtTenir::~Mission_Pawn_MILICE_PrendreEtTenir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_PrendreEtTenir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_PrendreEtTenir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_MILICE_PrendreEtTenir& asnMission = *new Mission_Pion_MILICE_PrendreEtTenir();

    asnMsg_.GetAsnMsg().mission().mission_pion_milice_prendre_et_tenir = &asnMission;

    const Position& pointAPrendre_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointAPrendre_, asnMission.point_a_prendre );

    delete &pointAPrendre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_PrendreEtTenir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_PrendreEtTenir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_milice_prendre_et_tenir ());
    Mission_Pion_MILICE_PrendreEtTenir& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_milice_prendre_et_tenir;

    ASN_Tools::Delete( asnMission.point_a_prendre );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

