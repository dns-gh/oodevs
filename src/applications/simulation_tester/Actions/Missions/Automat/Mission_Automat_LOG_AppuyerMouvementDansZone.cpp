// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_AppuyerMouvementDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvementDansZone::Mission_Automat_LOG_AppuyerMouvementDansZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_AppuyerMouvementDansZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvementDansZone::~Mission_Automat_LOG_AppuyerMouvementDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvementDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_LOG_AppuyerMouvementDansZone& asnMission = *new Mission_Automate_LOG_AppuyerMouvementDansZone();

    asnMsg_.GetAsnMsg().mission().mission_automate_log_appuyer_mouvement_dans_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvementDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_log_appuyer_mouvement_dans_zone ());
    Mission_Automate_LOG_AppuyerMouvementDansZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_log_appuyer_mouvement_dans_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




