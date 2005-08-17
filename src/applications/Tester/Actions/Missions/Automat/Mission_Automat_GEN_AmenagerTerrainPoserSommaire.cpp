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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_AmenagerTerrainPoserSommaire";
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

    ASN_Tools::CopyGenObject( pTarget_->GetTestParam_GenObject(), asnMission.terrain );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_AmenagerTerrainPoserSommaire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire );
    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_amenager_terrain_poser_sommaire;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




