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
#include "Mission_Automat_GEN_RealiserSystemeObstacles.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserSystemeObstacles constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserSystemeObstacles::Mission_Automat_GEN_RealiserSystemeObstacles( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_RealiserSystemeObstacles";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserSystemeObstacles destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserSystemeObstacles::~Mission_Automat_GEN_RealiserSystemeObstacles()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserSystemeObstacles::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserSystemeObstacles::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_systeme_obstacles = &asnMission;

    const T_IdVector& obstacles_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( obstacles_, asnMission.obstacles );

    delete &obstacles_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserSystemeObstacles::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserSystemeObstacles::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_systeme_obstacles );
    ASN1T_Mission_Automate_GEN_RealiserSystemeObstacles& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_systeme_obstacles;

    ASN_Tools::Delete( asnMission.obstacles );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




