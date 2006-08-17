// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_RecueillirUnite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RecueillirUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_RecueillirUnite::Mission_Automat_INF_RecueillirUnite( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_RecueillirUnite", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RecueillirUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_RecueillirUnite::~Mission_Automat_INF_RecueillirUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RecueillirUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_RecueillirUnite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *new ASN1T_Mission_Automate_INF_RecueillirUnite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_recueillir_unite;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_recueillir_unite = &asnMission;

    const Location& zoneDeploiement_ = pTarget_->GetTestParam_Polygon();
    const T_PositionVector& pias_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPolygon( zoneDeploiement_, asnMission.zone_deploiement );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );
    ASN_Tools::CopyPointList( pias_, asnMission.pias );

    delete &zoneDeploiement_;
    delete &pias_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RecueillirUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_RecueillirUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_recueillir_unite );
    ASN1T_Mission_Automate_INF_RecueillirUnite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_recueillir_unite;

    ASN_Tools::Delete( asnMission.zone_deploiement );
    ASN_Tools::Delete( asnMission.pias );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




