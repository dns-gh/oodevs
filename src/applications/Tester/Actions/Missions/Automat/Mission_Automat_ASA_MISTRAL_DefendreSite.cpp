// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ASA_MISTRAL_DefendreSite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_MISTRAL_DefendreSite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_MISTRAL_DefendreSite::Mission_Automat_ASA_MISTRAL_DefendreSite( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASA_MISTRAL_DefendreSite", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_MISTRAL_DefendreSite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_MISTRAL_DefendreSite::~Mission_Automat_ASA_MISTRAL_DefendreSite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_MISTRAL_DefendreSite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_MISTRAL_DefendreSite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_mistral_defendre_site;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_site = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 5 ), asnMission.mode_deploiement );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric( 0, 360 ), asnMission.angle );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_MISTRAL_DefendreSite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_MISTRAL_DefendreSite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_asa_mistral_defendre_site );
    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreSite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_mistral_defendre_site;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




