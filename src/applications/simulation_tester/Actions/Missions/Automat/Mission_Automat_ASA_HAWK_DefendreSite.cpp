// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ASA_HAWK_DefendreSite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_HAWK_DefendreSite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_HAWK_DefendreSite::Mission_Automat_ASA_HAWK_DefendreSite( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASA_HAWK_DefendreSite", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_HAWK_DefendreSite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_HAWK_DefendreSite::~Mission_Automat_ASA_HAWK_DefendreSite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_HAWK_DefendreSite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_HAWK_DefendreSite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *new Mission_Automate_ASA_HAWK_DefendreSite();

    asnMsg_.GetAsnMsg().mission().mission_automate_asa_hawk_defendre_site = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const T_PositionVector& positionsSections_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPointList( positionsSections_, asnMission.positions_sections );

    delete &zone_;
    delete &positionsSections_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_HAWK_DefendreSite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_HAWK_DefendreSite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_asa_hawk_defendre_site ());
    Mission_Automate_ASA_HAWK_DefendreSite& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_asa_hawk_defendre_site;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.positions_sections );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




