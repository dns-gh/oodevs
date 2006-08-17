// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_AmenagerTerrainPoserSommaire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerTerrainPoserSommaire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Mission_Automat_GEN_AmenagerTerrainPoserSommaire( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_AmenagerTerrainPoserSommaire", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerTerrainPoserSommaire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_AmenagerTerrainPoserSommaire::~Mission_Automat_GEN_AmenagerTerrainPoserSommaire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *new ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_amenager_terrain_poser_sommaire = &asnMission;

    const GenObject& terrain_ = pTarget_->GetTestParam_GenObject();

    ASN_Tools::CopyGenObject( terrain_, asnMission.terrain );

    delete &terrain_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire );
    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_amenager_terrain_poser_sommaire;

    ASN_Tools::Delete( asnMission.terrain );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




