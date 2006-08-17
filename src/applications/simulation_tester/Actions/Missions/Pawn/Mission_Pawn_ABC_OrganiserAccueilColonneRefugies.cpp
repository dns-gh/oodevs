// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ABC_OrganiserAccueilColonneRefugies.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OrganiserAccueilColonneRefugies constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::Mission_Pawn_ABC_OrganiserAccueilColonneRefugies( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ABC_OrganiserAccueilColonneRefugies", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OrganiserAccueilColonneRefugies destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::~Mission_Pawn_ABC_OrganiserAccueilColonneRefugies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& asnMission = *new ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies = &asnMission;

    const Position& positionInstallation_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionInstallation_, asnMission.position_installation );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_OrganiserAccueilColonneRefugies::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_organiser_accueil_colonne_refugies );
    ASN1T_Mission_Pion_ABC_OrganiserAccueilColonneRefugies& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_organiser_accueil_colonne_refugies;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


