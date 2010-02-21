// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_ReconnaitreZoneContaminee.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneContaminee constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ReconnaitreZoneContaminee::Mission_Automat_LOG_ReconnaitreZoneContaminee( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_ReconnaitreZoneContaminee", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneContaminee destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ReconnaitreZoneContaminee::~Mission_Automat_LOG_ReconnaitreZoneContaminee()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneContaminee::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ReconnaitreZoneContaminee::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_LOG_ReconnaitreZoneContaminee& asnMission = *new Mission_Automate_LOG_ReconnaitreZoneContaminee();

    asnMsg_.GetAsnMsg().mission().mission_automate_log_reconnaitre_zone_contaminee = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ReconnaitreZoneContaminee::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ReconnaitreZoneContaminee::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_log_reconnaitre_zone_contaminee ());
    Mission_Automate_LOG_ReconnaitreZoneContaminee& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_log_reconnaitre_zone_contaminee;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




