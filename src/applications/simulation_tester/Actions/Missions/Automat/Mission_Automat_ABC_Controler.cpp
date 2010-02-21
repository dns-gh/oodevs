// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ABC_Controler.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Controler::Mission_Automat_ABC_Controler( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_Controler", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_Controler::~Mission_Automat_ABC_Controler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Controler::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ABC_Controler& asnMission = *new Mission_Automate_ABC_Controler();

    asnMsg_.GetAsnMsg().mission().mission_automate_abc_controler = &asnMission;

    const Location& zoneAControler_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneAControler_, asnMission.zone_a_controler );

    delete &zoneAControler_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_Controler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_Controler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_abc_controler ());
    Mission_Automate_ABC_Controler& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_abc_controler;

    ASN_Tools::Delete( asnMission.zone_a_controler );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




