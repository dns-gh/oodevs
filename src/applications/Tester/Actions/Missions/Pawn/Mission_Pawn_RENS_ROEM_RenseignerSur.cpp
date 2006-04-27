// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_RENS_ROEM_RenseignerSur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_RenseignerSur::Mission_Pawn_RENS_ROEM_RenseignerSur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_ROEM_RenseignerSur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_RenseignerSur::~Mission_Pawn_RENS_ROEM_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_renseigner_sur = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_roem_renseigner_sur );
    ASN1T_Mission_Pion_RENS_ROEM_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_renseigner_sur;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


