// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_LOG_DistribuerRessources.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerRessources constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DistribuerRessources::Mission_Pawn_LOG_DistribuerRessources( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_LOG_DistribuerRessources", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerRessources destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_LOG_DistribuerRessources::~Mission_Pawn_LOG_DistribuerRessources()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerRessources::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DistribuerRessources::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_LOG_DistribuerRessources& asnMission = *new ASN1T_Mission_Pion_LOG_DistribuerRessources();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_log_distribuer_ressources;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_log_distribuer_ressources = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_LOG_DistribuerRessources::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_LOG_DistribuerRessources::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_log_distribuer_ressources );
    ASN1T_Mission_Pion_LOG_DistribuerRessources& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_log_distribuer_ressources;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


