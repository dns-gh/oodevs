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
#include "Mission_Automat_INF_BarrerDirection.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_BarrerDirection::Mission_Automat_INF_BarrerDirection( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_BarrerDirection";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_BarrerDirection::~Mission_Automat_INF_BarrerDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_BarrerDirection::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_BarrerDirection& asnMission = *new ASN1T_Mission_Automate_INF_BarrerDirection();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_barrer_direction;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_barrer_direction = &asnMission;

    const Location& position_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( position_, asnMission.position );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &position_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_BarrerDirection::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_BarrerDirection::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_barrer_direction );
    ASN1T_Mission_Automate_INF_BarrerDirection& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_barrer_direction;

    ASN_Tools::Delete( asnMission.position );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




