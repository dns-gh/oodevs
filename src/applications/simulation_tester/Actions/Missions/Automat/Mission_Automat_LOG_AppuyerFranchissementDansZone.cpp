// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_LOG_AppuyerFranchissementDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerFranchissementDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerFranchissementDansZone::Mission_Automat_LOG_AppuyerFranchissementDansZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_AppuyerFranchissementDansZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerFranchissementDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerFranchissementDansZone::~Mission_Automat_LOG_AppuyerFranchissementDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerFranchissementDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerFranchissementDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_franchissement_dans_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerFranchissementDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerFranchissementDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_appuyer_franchissement_dans_zone );
    ASN1T_Mission_Automate_LOG_AppuyerFranchissementDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_franchissement_dans_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




