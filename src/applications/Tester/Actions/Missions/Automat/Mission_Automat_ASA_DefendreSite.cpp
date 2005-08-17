// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Automat_ASA_DefendreSite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreSite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreSite::Mission_Automat_ASA_DefendreSite( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ASA_DefendreSite";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreSite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreSite::~Mission_Automat_ASA_DefendreSite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreSite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreSite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreSite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_site;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_site = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );
    ASN_Tools::CopyNumeric( pTarget_->GetTestParam_Numeric(), asnMission.angle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreSite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreSite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_asa_defendre_site );
    ASN1T_Mission_Automate_ASA_DefendreSite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_site;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




