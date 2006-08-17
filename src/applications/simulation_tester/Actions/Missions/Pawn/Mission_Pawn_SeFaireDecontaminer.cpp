// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_SeFaireDecontaminer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireDecontaminer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SeFaireDecontaminer::Mission_Pawn_SeFaireDecontaminer( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_SeFaireDecontaminer", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireDecontaminer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SeFaireDecontaminer::~Mission_Pawn_SeFaireDecontaminer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireDecontaminer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_SeFaireDecontaminer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Pion_SeFaireDecontaminer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_decontaminer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer = &asnMission;

    const Position& pointArrivee_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointArrivee_, asnMission.point_arrivee );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_decontamination );

    delete &pointArrivee_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireDecontaminer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_SeFaireDecontaminer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_se_faire_decontaminer );
    ASN1T_Mission_Pion_SeFaireDecontaminer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_se_faire_decontaminer;

    ASN_Tools::Delete( asnMission.point_arrivee );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


