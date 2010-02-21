// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_ReconnaitreZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_ReconnaitreZone::Mission_Automat_GEN_ReconnaitreZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_ReconnaitreZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_ReconnaitreZone::~Mission_Automat_GEN_ReconnaitreZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_ReconnaitreZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_GEN_ReconnaitreZone& asnMission = *new Mission_Automate_GEN_ReconnaitreZone();

    asnMsg_.GetAsnMsg().mission().mission_automate_gen_reconnaitre_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_ReconnaitreZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_ReconnaitreZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_gen_reconnaitre_zone ());
    Mission_Automate_GEN_ReconnaitreZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_gen_reconnaitre_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




