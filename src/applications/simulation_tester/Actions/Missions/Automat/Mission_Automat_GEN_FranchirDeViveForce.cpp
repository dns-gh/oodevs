// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_FranchirDeViveForce.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FranchirDeViveForce constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_FranchirDeViveForce::Mission_Automat_GEN_FranchirDeViveForce( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_FranchirDeViveForce", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FranchirDeViveForce destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_FranchirDeViveForce::~Mission_Automat_GEN_FranchirDeViveForce()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FranchirDeViveForce::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_FranchirDeViveForce::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *new ASN1T_Mission_Automate_GEN_FranchirDeViveForce();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_franchir_de_vive_force;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_franchir_de_vive_force = &asnMission;

    const T_IdVector& obstaclesAFranchir_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( obstaclesAFranchir_, asnMission.obstacles_a_franchir );

    delete &obstaclesAFranchir_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FranchirDeViveForce::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_FranchirDeViveForce::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_franchir_de_vive_force );
    ASN1T_Mission_Automate_GEN_FranchirDeViveForce& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_franchir_de_vive_force;

    ASN_Tools::Delete( asnMission.obstacles_a_franchir );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




