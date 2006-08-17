// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ASA_Surveiller.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_Surveiller::Mission_Automat_ASA_Surveiller( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ASA_Surveiller", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_Surveiller::~Mission_Automat_ASA_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_Surveiller();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_surveiller;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_surveiller = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const T_PositionVector& positionsSections_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPointList( positionsSections_, asnMission.positions_sections );

    delete &zone_;
    delete &positionsSections_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_asa_surveiller );
    ASN1T_Mission_Automate_ASA_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_surveiller;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.positions_sections );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




