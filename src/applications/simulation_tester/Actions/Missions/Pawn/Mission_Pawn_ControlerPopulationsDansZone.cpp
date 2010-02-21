// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ControlerPopulationsDansZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ControlerPopulationsDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ControlerPopulationsDansZone::Mission_Pawn_ControlerPopulationsDansZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ControlerPopulationsDansZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ControlerPopulationsDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ControlerPopulationsDansZone::~Mission_Pawn_ControlerPopulationsDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ControlerPopulationsDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ControlerPopulationsDansZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ControlerPopulationsDansZone& asnMission = *new Mission_Pion_ControlerPopulationsDansZone();

    asnMsg_.GetAsnMsg().mission().mission_pion_controler_populations_dans_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ControlerPopulationsDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ControlerPopulationsDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_controler_populations_dans_zone ());
    Mission_Pion_ControlerPopulationsDansZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_controler_populations_dans_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


