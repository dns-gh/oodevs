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
#include "Mission_Automat_ABC_Reconnaitre.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reconnaitre::Mission_Automat_ABC_Reconnaitre( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_Reconnaitre";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Reconnaitre::~Mission_Automat_ABC_Reconnaitre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reconnaitre::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Reconnaitre& asnMission = *new ASN1T_Mission_Automate_ABC_Reconnaitre();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_reconnaitre;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_reconnaitre = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Reconnaitre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Reconnaitre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_reconnaitre );
    ASN1T_Mission_Automate_ABC_Reconnaitre& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_reconnaitre;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




