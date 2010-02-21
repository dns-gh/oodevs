// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_RENS_ROIM_MettreEnOeuvre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROIM_MettreEnOeuvre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROIM_MettreEnOeuvre::Mission_Pawn_RENS_ROIM_MettreEnOeuvre( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_RENS_ROIM_MettreEnOeuvre", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROIM_MettreEnOeuvre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROIM_MettreEnOeuvre::~Mission_Pawn_RENS_ROIM_MettreEnOeuvre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROIM_MettreEnOeuvre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROIM_MettreEnOeuvre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *new Mission_Pion_RENS_ROIM_MettreEnOeuvre();

    asnMsg_.GetAsnMsg().mission().mission_pion_rens_roim_mettre_en_oeuvre = &asnMission;

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROIM_MettreEnOeuvre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROIM_MettreEnOeuvre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_rens_roim_mettre_en_oeuvre ());
    Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_rens_roim_mettre_en_oeuvre;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


