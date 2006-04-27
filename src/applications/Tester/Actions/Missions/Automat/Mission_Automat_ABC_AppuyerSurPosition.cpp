// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ABC_AppuyerSurPosition.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_AppuyerSurPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_AppuyerSurPosition::Mission_Automat_ABC_AppuyerSurPosition( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_AppuyerSurPosition", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_AppuyerSurPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_AppuyerSurPosition::~Mission_Automat_ABC_AppuyerSurPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_AppuyerSurPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_AppuyerSurPosition::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_AppuyerSurPosition& asnMission = *new ASN1T_Mission_Automate_ABC_AppuyerSurPosition();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_appuyer_sur_position;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_appuyer_sur_position = &asnMission;

    const Location& positionInstallation_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( positionInstallation_, asnMission.position_installation );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_AppuyerSurPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_AppuyerSurPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_appuyer_sur_position );
    ASN1T_Mission_Automate_ABC_AppuyerSurPosition& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_appuyer_sur_position;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




