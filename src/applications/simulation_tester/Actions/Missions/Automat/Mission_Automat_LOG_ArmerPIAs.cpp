// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_LOG_ArmerPIAs.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ArmerPIAs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ArmerPIAs::Mission_Automat_LOG_ArmerPIAs( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_LOG_ArmerPIAs", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ArmerPIAs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_ArmerPIAs::~Mission_Automat_LOG_ArmerPIAs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ArmerPIAs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ArmerPIAs::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_ArmerPIAs& asnMission = *new ASN1T_Mission_Automate_LOG_ArmerPIAs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_armer_pias;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_armer_pias = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_ArmerPIAs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_ArmerPIAs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_armer_pias );
    ASN1T_Mission_Automate_LOG_ArmerPIAs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_armer_pias;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




