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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_SecuriserZoneContrePopulations";
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

    ASN1T_Mission_Automate_SecuriserZoneContrePopulations& asnMission = *new ASN1T_Mission_Automate_SecuriserZoneContrePopulations();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_securiser_zone_contre_populations;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_securiser_zone_contre_populations = &asnMission;

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
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_securiser_zone_contre_populations );
    ASN1T_Mission_Automate_SecuriserZoneContrePopulations& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_securiser_zone_contre_populations;

    ASN_Tools::Delete( asnMission.points );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




