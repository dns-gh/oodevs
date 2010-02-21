// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_AttaquerSouplesse.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerSouplesse::Mission_Automat_INF_AttaquerSouplesse( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_AttaquerSouplesse", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerSouplesse::~Mission_Automat_INF_AttaquerSouplesse()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerSouplesse::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_AttaquerSouplesse& asnMission = *new Mission_Automate_INF_AttaquerSouplesse();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_attaquer_souplesse = &asnMission;

    const T_PositionVector& objectifs_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( objectifs_, asnMission.objectifs );

    delete &objectifs_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerSouplesse::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_attaquer_souplesse ());
    Mission_Automate_INF_AttaquerSouplesse& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_attaquer_souplesse;

    ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




