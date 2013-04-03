// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_ReconnaitrePoint.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReconnaitrePoint constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ReconnaitrePoint::Mission_Automat_ABC_ReconnaitrePoint( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_ReconnaitrePoint", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReconnaitrePoint destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ReconnaitrePoint::~Mission_Automat_ABC_ReconnaitrePoint()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReconnaitrePoint::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ReconnaitrePoint::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_ReconnaitrePoint& asnMission = *new Mission_Automate_ABC_ReconnaitrePoint();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_reconnaitre_point = &asnMission;

    const Location& pointAReconnaitre_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( pointAReconnaitre_, asnMission.point_a_reconnaitre );

    delete &pointAReconnaitre_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReconnaitrePoint::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ReconnaitrePoint::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_reconnaitre_point ());
    Mission_Automate_ABC_ReconnaitrePoint& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_reconnaitre_point;

    ASN_Tools::Delete( asnMission.point_a_reconnaitre );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



