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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_Couvrir";
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

    ASN1T_Mission_Automate_ABC_Couvrir& asnMission = *new ASN1T_Mission_Automate_ABC_Couvrir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_couvrir;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_couvrir = &asnMission;

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
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_couvrir );
    ASN1T_Mission_Automate_ABC_Couvrir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_couvrir;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




