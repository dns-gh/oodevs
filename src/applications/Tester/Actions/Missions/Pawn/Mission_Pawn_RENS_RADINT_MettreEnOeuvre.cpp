// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_RENS_RADINT_MettreEnOeuvre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_MettreEnOeuvre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_MettreEnOeuvre::Mission_Pawn_RENS_RADINT_MettreEnOeuvre( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_RADINT_MettreEnOeuvre", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_MettreEnOeuvre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_MettreEnOeuvre::~Mission_Pawn_RENS_RADINT_MettreEnOeuvre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_MettreEnOeuvre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_MettreEnOeuvre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_mettre_en_oeuvre = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_MettreEnOeuvre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_MettreEnOeuvre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_radint_mettre_en_oeuvre );
    ASN1T_Mission_Pion_RENS_RADINT_MettreEnOeuvre& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_mettre_en_oeuvre;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


