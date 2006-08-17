// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASY_CommettreAttentatContrePopulation.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContrePopulation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContrePopulation::Mission_Pawn_ASY_CommettreAttentatContrePopulation( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASY_CommettreAttentatContrePopulation", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContrePopulation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContrePopulation::~Mission_Pawn_ASY_CommettreAttentatContrePopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContrePopulation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContrePopulation::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContrePopulation = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointRepli_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointRepli_, asnMission.point_repli );
    ASN_Tools::CopyPopulationKnowledge( pTarget_->GetTestParam_PopulationKnowledge(), asnMission.population_cible );

    delete &itineraire_;
    delete &pointRepli_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContrePopulation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContrePopulation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentatContrePopulation );
    ASN1T_Mission_Pion_ASY_CommettreAttentatContrePopulation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContrePopulation;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


