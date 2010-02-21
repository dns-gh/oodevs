// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_EvacuationSanitaire.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EvacuationSanitaire constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_EvacuationSanitaire::Mission_Pawn_ALAT_EvacuationSanitaire( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_EvacuationSanitaire", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EvacuationSanitaire destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_EvacuationSanitaire::~Mission_Pawn_ALAT_EvacuationSanitaire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EvacuationSanitaire::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_EvacuationSanitaire::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *new Mission_Pion_ALAT_EvacuationSanitaire();

    asnMsg_.GetAsnMsg().mission().mission_pion_alat_evacuation_sanitaire = &asnMission;

    const Position& pointDebarquement_ = pTarget_->GetTestParam_Point();
    const Location& zoneExtraction_ = pTarget_->GetTestParam_Polygon();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPoint( pointDebarquement_, asnMission.point_debarquement );
    ASN_Tools::CopyPolygon( zoneExtraction_, asnMission.zone_extraction );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );

    delete &pointDebarquement_;
    delete &zoneExtraction_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_EvacuationSanitaire::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_EvacuationSanitaire::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_alat_evacuation_sanitaire ());
    Mission_Pion_ALAT_EvacuationSanitaire& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_alat_evacuation_sanitaire;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.zone_extraction );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


