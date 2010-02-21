// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Automat_ABC ( "Automat_LOG_AppuyerMouvement", Automat )
{
    // NOTHING
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

    Mission_Automate_LOG_AppuyerMouvement& asnMission = *new Mission_Automate_LOG_AppuyerMouvement();

    asnMsg_.GetAsnMsg().mission().mission_automate_log_appuyer_mouvement = &asnMission;

    const T_IdVector& unitesAAppuyer_ = pTarget_->GetTestParam_AgentList();

    ASN_Tools::CopyAgentList( unitesAAppuyer_, asnMission.unites_a_appuyer );

    delete &unitesAAppuyer_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_log_appuyer_mouvement ());
    Mission_Automate_LOG_AppuyerMouvement& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_log_appuyer_mouvement;

    ASN_Tools::Delete( asnMission.unites_a_appuyer );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




