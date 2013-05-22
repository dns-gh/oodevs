// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_RompreContact.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RompreContact constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_RompreContact::Mission_Automat_INF_RompreContact( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_RompreContact", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RompreContact destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_RompreContact::~Mission_Automat_INF_RompreContact()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RompreContact::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_RompreContact::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_RompreContact& asnMission = *new Mission_Automate_INF_RompreContact();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_rompre_contact = &asnMission;

    const Location& zoneRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneRegroupement_, asnMission.zone_regroupement );

    delete &zoneRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_RompreContact::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_RompreContact::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_rompre_contact ());
    Mission_Automate_INF_RompreContact& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_rompre_contact;

    ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



