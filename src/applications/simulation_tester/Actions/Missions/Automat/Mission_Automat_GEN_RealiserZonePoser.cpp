// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_GEN_RealiserZonePoser.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserZonePoser::Mission_Automat_GEN_RealiserZonePoser( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RealiserZonePoser", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserZonePoser::~Mission_Automat_GEN_RealiserZonePoser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserZonePoser::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserZonePoser& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserZonePoser();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_zone_poser;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser = &asnMission;

    const GenObject& zone_ = pTarget_->GetTestParam_GenObject();

    ASN_Tools::CopyGenObject( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserZonePoser::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_zone_poser );
    ASN1T_Mission_Automate_GEN_RealiserZonePoser& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




