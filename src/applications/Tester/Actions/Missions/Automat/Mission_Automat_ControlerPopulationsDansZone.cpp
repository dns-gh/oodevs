// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ControlerPopulationsDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ControlerPopulationsDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ControlerPopulationsDansZone::Mission_Automat_ControlerPopulationsDansZone( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ControlerPopulationsDansZone", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ControlerPopulationsDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ControlerPopulationsDansZone::~Mission_Automat_ControlerPopulationsDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ControlerPopulationsDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ControlerPopulationsDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ControlerPopulationsDansZone& asnMission = *new ASN1T_Mission_Automate_ControlerPopulationsDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_controler_populations_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_controler_populations_dans_zone = &asnMission;

    const T_LocationVector& zones_ = pTarget_->GetTestParam_PolygonList();

    ASN_Tools::CopyPolygonList( zones_, asnMission.zones );

    delete &zones_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ControlerPopulationsDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ControlerPopulationsDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_controler_populations_dans_zone );
    ASN1T_Mission_Automate_ControlerPopulationsDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_controler_populations_dans_zone;

    ASN_Tools::Delete( asnMission.zones );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




