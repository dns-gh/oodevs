// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Surveiller.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Surveiller::Mission_Automat_INF_Surveiller( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Surveiller", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Surveiller::~Mission_Automat_INF_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Surveiller& asnMission = *new ASN1T_Mission_Automate_INF_Surveiller();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_surveiller;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_surveiller = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Location& zoneRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPolygon( zoneRegroupement_, asnMission.zone_regroupement );

    delete &zone_;
    delete &zoneRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_surveiller );
    ASN1T_Mission_Automate_INF_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_surveiller;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




