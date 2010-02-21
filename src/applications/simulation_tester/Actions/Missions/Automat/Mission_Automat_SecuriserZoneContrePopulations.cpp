// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_SecuriserZoneContrePopulations.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SecuriserZoneContrePopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SecuriserZoneContrePopulations::Mission_Automat_SecuriserZoneContrePopulations( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_SecuriserZoneContrePopulations", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SecuriserZoneContrePopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SecuriserZoneContrePopulations::~Mission_Automat_SecuriserZoneContrePopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SecuriserZoneContrePopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SecuriserZoneContrePopulations::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_SecuriserZoneContrePopulations& asnMission = *new Mission_Automate_SecuriserZoneContrePopulations();

    asnMsg_.GetAsnMsg().mission().mission_automate_securiser_zone_contre_populations = &asnMission;

    const T_PositionVector& points_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( points_, asnMission.points );

    delete &points_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SecuriserZoneContrePopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SecuriserZoneContrePopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_securiser_zone_contre_populations ());
    Mission_Automate_SecuriserZoneContrePopulations& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_securiser_zone_contre_populations;

    ASN_Tools::Delete( asnMission.points );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




