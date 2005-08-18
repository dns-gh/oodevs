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
#include "Mission_Automat_ABC_Attaquer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Attaquer::Mission_Automat_ABC_Attaquer( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_Attaquer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Attaquer::~Mission_Automat_ABC_Attaquer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Attaquer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Attaquer& asnMission = *new ASN1T_Mission_Automate_ABC_Attaquer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_attaquer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_attaquer = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Attaquer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Attaquer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_attaquer );
    ASN1T_Mission_Automate_ABC_Attaquer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_attaquer;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




