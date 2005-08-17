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
#include "Mission_Automat_ABC_Controler.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Controler::Mission_Automat_ABC_Controler( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_Controler";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Controler::~Mission_Automat_ABC_Controler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Controler::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Controler& asnMission = *new ASN1T_Mission_Automate_ABC_Controler();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_controler;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_controler = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_a_controler );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Controler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_controler );
    ASN1T_Mission_Automate_ABC_Controler& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_controler;

    ASN_Tools::Delete( asnMission.zone_a_controler );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




