// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ASS_ReconnaitreZonesDeploiement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_ReconnaitreZonesDeploiement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_ReconnaitreZonesDeploiement::Mission_Automat_ASS_ReconnaitreZonesDeploiement( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASS_ReconnaitreZonesDeploiement", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_ReconnaitreZonesDeploiement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_ReconnaitreZonesDeploiement::~Mission_Automat_ASS_ReconnaitreZonesDeploiement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_ReconnaitreZonesDeploiement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_ReconnaitreZonesDeploiement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *new ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_ass_reconnaitre_zones_deploiement = &asnMission;

    const T_PositionVector& positionsAReconnaitre_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( positionsAReconnaitre_, asnMission.positions_a_reconnaitre );

    delete &positionsAReconnaitre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_ReconnaitreZonesDeploiement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_ReconnaitreZonesDeploiement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement );
    ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_ass_reconnaitre_zones_deploiement;

    ASN_Tools::Delete( asnMission.positions_a_reconnaitre );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




