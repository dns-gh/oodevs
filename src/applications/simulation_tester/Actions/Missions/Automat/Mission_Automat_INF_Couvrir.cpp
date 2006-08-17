// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Couvrir.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Couvrir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Couvrir::Mission_Automat_INF_Couvrir( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Couvrir", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Couvrir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Couvrir::~Mission_Automat_INF_Couvrir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Couvrir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Couvrir::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_Couvrir& asnMission = *new ASN1T_Mission_Automate_INF_Couvrir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_couvrir;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_couvrir = &asnMission;

    const T_PositionVector& objectifs_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );
    ASN_Tools::CopyPointList( objectifs_, asnMission.objectifs );

    delete &objectifs_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Couvrir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Couvrir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_couvrir );
    ASN1T_Mission_Automate_INF_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_couvrir;

    ASN_Tools::Delete( asnMission.objectifs );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




