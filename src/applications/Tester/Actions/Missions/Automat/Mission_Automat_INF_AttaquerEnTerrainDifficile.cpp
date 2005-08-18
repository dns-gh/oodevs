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
#include "Mission_Automat_INF_AttaquerEnTerrainDifficile.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerEnTerrainDifficile constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerEnTerrainDifficile::Mission_Automat_INF_AttaquerEnTerrainDifficile( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_AttaquerEnTerrainDifficile";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerEnTerrainDifficile destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerEnTerrainDifficile::~Mission_Automat_INF_AttaquerEnTerrainDifficile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerEnTerrainDifficile::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerEnTerrainDifficile::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_attaquer_en_terrain_difficile = &asnMission;

    const T_PositionVector& objectifs_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( objectifs_, asnMission.objectifs );

    delete &objectifs_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerEnTerrainDifficile::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerEnTerrainDifficile::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_attaquer_en_terrain_difficile );
    ASN1T_Mission_Automate_INF_AttaquerEnTerrainDifficile& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_attaquer_en_terrain_difficile;

    ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




