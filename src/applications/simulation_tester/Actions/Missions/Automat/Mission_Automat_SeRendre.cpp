// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_SeRendre.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRendre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeRendre::Mission_Automat_SeRendre( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_SeRendre", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRendre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeRendre::~Mission_Automat_SeRendre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRendre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SeRendre::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_SeRendre& asnMission = *new ASN1T_Mission_Automate_SeRendre();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_rendre;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_se_rendre = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeRendre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SeRendre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_se_rendre );
    ASN1T_Mission_Automate_SeRendre& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_se_rendre;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




