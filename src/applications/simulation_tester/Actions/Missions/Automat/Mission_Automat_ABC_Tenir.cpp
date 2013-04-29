// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Tenir.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Tenir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Tenir::Mission_Automat_ABC_Tenir( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Tenir", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Tenir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Tenir::~Mission_Automat_ABC_Tenir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Tenir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Tenir::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Tenir& asnMission = *new Mission_Automate_ABC_Tenir();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_tenir = &asnMission;

    const Location& positionInstallation_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( positionInstallation_, asnMission.position_installation );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Tenir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Tenir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_tenir ());
    Mission_Automate_ABC_Tenir& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_tenir;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



