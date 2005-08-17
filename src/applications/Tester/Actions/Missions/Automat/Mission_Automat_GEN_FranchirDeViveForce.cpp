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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_FranchirDeViveForce";
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

    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.obstacles_a_franchir );

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




