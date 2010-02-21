// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Couvrir.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Couvrir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Couvrir::Mission_Automat_ABC_Couvrir( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Couvrir", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Couvrir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Couvrir::~Mission_Automat_ABC_Couvrir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Couvrir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Couvrir::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Couvrir& asnMission = *new Mission_Automate_ABC_Couvrir();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_couvrir = &asnMission;

    const Location& positionInstallation_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( positionInstallation_, asnMission.position_installation );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Couvrir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Couvrir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_couvrir ());
    Mission_Automate_ABC_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_couvrir;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




