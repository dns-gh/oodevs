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
#include "Mission_Automat_NBC_DecontaminerUneZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_DecontaminerUneZone::Mission_Automat_NBC_DecontaminerUneZone( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_NBC_DecontaminerUneZone";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_DecontaminerUneZone::~Mission_Automat_NBC_DecontaminerUneZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_DecontaminerUneZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *new ASN1T_Mission_Automate_NBC_DecontaminerUneZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_decontaminer_une_zone = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_DecontaminerUneZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_DecontaminerUneZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_nbc_decontaminer_une_zone );
    ASN1T_Mission_Automate_NBC_DecontaminerUneZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_decontaminer_une_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




