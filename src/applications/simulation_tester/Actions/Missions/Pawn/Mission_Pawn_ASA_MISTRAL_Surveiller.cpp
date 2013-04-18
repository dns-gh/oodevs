// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASA_MISTRAL_Surveiller.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_Surveiller::Mission_Pawn_ASA_MISTRAL_Surveiller( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASA_MISTRAL_Surveiller", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_Surveiller::~Mission_Pawn_ASA_MISTRAL_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *new Mission_Pion_ASA_MISTRAL_Surveiller();

    asnMsg_.GetAsnMsg().mission().mission_pion_asa_mistral_surveiller = &asnMission;

    const Position& pointDeDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( pointDeDeploiement_, asnMission.point_de_deploiement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.mode_deploiement );

    delete &pointDeDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_asa_mistral_surveiller ());
    Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_asa_mistral_surveiller;

    ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

