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
#include "Mission_Automat_ABC_EscorterUnite.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnite constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnite::Mission_Automat_ABC_EscorterUnite( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_EscorterUnite";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnite destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnite::~Mission_Automat_ABC_EscorterUnite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnite::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnite::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_EscorterUnite& asnMission = *new ASN1T_Mission_Automate_ABC_EscorterUnite();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_escorter_unite;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_escorter_unite = &asnMission;


    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.unite_a_escorter );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnite::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnite::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_escorter_unite );
    ASN1T_Mission_Automate_ABC_EscorterUnite& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_escorter_unite;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




