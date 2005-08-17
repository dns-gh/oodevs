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
#include "Mission_Automat_ABC_Surveiller.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Surveiller::Mission_Automat_ABC_Surveiller( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_Surveiller";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Surveiller::~Mission_Automat_ABC_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Surveiller& asnMission = *new ASN1T_Mission_Automate_ABC_Surveiller();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_surveiller;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_surveiller = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_installation );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_surveiller );
    ASN1T_Mission_Automate_ABC_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_surveiller;

    ASN_Tools::Delete( asnMission.zone_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




