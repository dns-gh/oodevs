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
#include "Mission_Automat_INF_Interdire.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Interdire::Mission_Automat_INF_Interdire( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_Interdire";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Interdire::~Mission_Automat_INF_Interdire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Interdire::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Interdire& asnMission = *new ASN1T_Mission_Automate_INF_Interdire();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_interdire;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_interdire = &asnMission;

    ASN_Tools::CopyPointList( pTarget_->GetTestParam_PointList(), asnMission.points_a_interdire );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Interdire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Interdire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_interdire );
    ASN1T_Mission_Automate_INF_Interdire& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_interdire;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




