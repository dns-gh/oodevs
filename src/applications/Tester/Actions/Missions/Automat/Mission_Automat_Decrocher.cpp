// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_Decrocher.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Decrocher constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Decrocher::Mission_Automat_Decrocher( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_Decrocher", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Decrocher destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Decrocher::~Mission_Automat_Decrocher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Decrocher::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Decrocher::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_Decrocher& asnMission = *new ASN1T_Mission_Automate_Decrocher();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_decrocher;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_decrocher = &asnMission;

    const Location& pointRegroupement_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( pointRegroupement_, asnMission.point_regroupement );

    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Decrocher::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Decrocher::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_decrocher );
    ASN1T_Mission_Automate_Decrocher& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_decrocher;

    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




