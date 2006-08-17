// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Fixer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Fixer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Fixer::Mission_Automat_ABC_Fixer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Fixer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Fixer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Fixer::~Mission_Automat_ABC_Fixer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Fixer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Fixer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_Fixer& asnMission = *new ASN1T_Mission_Automate_ABC_Fixer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_fixer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_fixer = &asnMission;

    const T_IdVector& ennemis_ = pTarget_->GetTestParam_AgentKnowledgeList();

    ASN_Tools::CopyAgentKnowledgeList( ennemis_, asnMission.ennemis );

    delete &ennemis_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Fixer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Fixer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_fixer );
    ASN1T_Mission_Automate_ABC_Fixer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_fixer;

    ASN_Tools::Delete( asnMission.ennemis );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




