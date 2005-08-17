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
#include "Mission_Automat_LOG_AppuyerMouvement.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvement::Mission_Automat_LOG_AppuyerMouvement( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_LOG_AppuyerMouvement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvement::~Mission_Automat_LOG_AppuyerMouvement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvement::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerMouvement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_mouvement;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement = &asnMission;

    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites_a_appuyer );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_appuyer_mouvement );
    ASN1T_Mission_Automate_LOG_AppuyerMouvement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




