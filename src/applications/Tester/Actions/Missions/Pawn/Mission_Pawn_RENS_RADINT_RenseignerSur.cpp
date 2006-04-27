// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_RENS_RADINT_RenseignerSur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_RenseignerSur::Mission_Pawn_RENS_RADINT_RenseignerSur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_RADINT_RenseignerSur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_RenseignerSur::~Mission_Pawn_RENS_RADINT_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur = &asnMission;

    const T_LocationVector& zonesDeRecherche_ = pTarget_->GetTestParam_PolygonList();

    ASN_Tools::CopyPolygonList( zonesDeRecherche_, asnMission.zones_de_recherche );

    delete &zonesDeRecherche_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_radint_renseigner_sur );
    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur;

    ASN_Tools::Delete( asnMission.zones_de_recherche );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


