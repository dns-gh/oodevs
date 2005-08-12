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
#include "Mission_Pawn_ASS_ReconnaitreZoneDeploiement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Mission_Pawn_ASS_ReconnaitreZoneDeploiement( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ASS_ReconnaitreZoneDeploiement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_ReconnaitreZoneDeploiement::~Mission_Pawn_ASS_ReconnaitreZoneDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *new ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_ReconnaitreZoneDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_reconnaitre_zone_deploiement );
    ASN1T_Mission_Pion_ASS_ReconnaitreZoneDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_reconnaitre_zone_deploiement;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


