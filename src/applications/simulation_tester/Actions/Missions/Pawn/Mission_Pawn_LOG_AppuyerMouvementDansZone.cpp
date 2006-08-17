// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_AppuyerMouvementDansZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvementDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerMouvementDansZone::Mission_Pawn_LOG_AppuyerMouvementDansZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_AppuyerMouvementDansZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvementDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_AppuyerMouvementDansZone::~Mission_Pawn_LOG_AppuyerMouvementDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvementDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerMouvementDansZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& asnMission = *new ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement_dans_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_AppuyerMouvementDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_AppuyerMouvementDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_appuyer_mouvement_dans_zone );
    ASN1T_Mission_Pion_LOG_AppuyerMouvementDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_appuyer_mouvement_dans_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


