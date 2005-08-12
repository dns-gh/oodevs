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
#include "Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_LOG_MettreEnOeuvreZoneStationnement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::~Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& asnMission = *new ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_mettre_en_oeuvre_zone_stationnement = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_MettreEnOeuvreZoneStationnement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_mettre_en_oeuvre_zone_stationnement );
    ASN1T_Mission_Pion_LOG_MettreEnOeuvreZoneStationnement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_mettre_en_oeuvre_zone_stationnement;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


