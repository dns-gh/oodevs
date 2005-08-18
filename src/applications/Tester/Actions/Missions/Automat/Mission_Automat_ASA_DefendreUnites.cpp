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
#include "Mission_Automat_ASA_DefendreUnites.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreUnites constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreUnites::Mission_Automat_ASA_DefendreUnites( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ASA_DefendreUnites";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreUnites destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASA_DefendreUnites::~Mission_Automat_ASA_DefendreUnites()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreUnites::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreUnites::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Automate_ASA_DefendreUnites();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_asa_defendre_unites;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_unites = &asnMission;

    const T_IdVector& unites_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyAgentList( unites_, asnMission.unites );

    delete &unites_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASA_DefendreUnites::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASA_DefendreUnites::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_asa_defendre_unites );
    ASN1T_Mission_Automate_ASA_DefendreUnites& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_asa_defendre_unites;

    ASN_Tools::Delete( asnMission.unites );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




