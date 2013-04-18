// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_LOG_BloquerZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_BloquerZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_BloquerZone::Mission_Pawn_LOG_BloquerZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_BloquerZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_BloquerZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_BloquerZone::~Mission_Pawn_LOG_BloquerZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_BloquerZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_BloquerZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_LOG_BloquerZone& asnMission = *new Mission_Pion_LOG_BloquerZone();

    asnMsg_.GetAsnMsg().mission().mission_pion_log_bloquer_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_BloquerZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_BloquerZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_log_bloquer_zone ());
    Mission_Pion_LOG_BloquerZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_log_bloquer_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

