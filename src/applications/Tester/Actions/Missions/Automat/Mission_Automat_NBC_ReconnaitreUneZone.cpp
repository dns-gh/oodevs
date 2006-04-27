// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_NBC_ReconnaitreUneZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUneZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreUneZone::Mission_Automat_NBC_ReconnaitreUneZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_NBC_ReconnaitreUneZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUneZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreUneZone::~Mission_Automat_NBC_ReconnaitreUneZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUneZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreUneZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreUneZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_une_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreUneZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreUneZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_nbc_reconnaitre_une_zone );
    ASN1T_Mission_Automate_NBC_ReconnaitreUneZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_une_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




