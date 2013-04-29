// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_INF_Controler.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Controler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Controler::Mission_Automat_INF_Controler( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_INF_Controler", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Controler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_Controler::~Mission_Automat_INF_Controler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Controler::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Controler::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_INF_Controler& asnMission = *new Mission_Automate_INF_Controler();

    asnMsg_.GetAsnMsg().mission().mission_automate_inf_controler = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_Controler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_Controler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_inf_controler ());
    Mission_Automate_INF_Controler& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_inf_controler;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



