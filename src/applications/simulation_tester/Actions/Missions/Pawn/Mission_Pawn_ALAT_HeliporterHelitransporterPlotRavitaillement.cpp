// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::~Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *new Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement();

    asnMsg_.GetAsnMsg().mission().mission_pion_alat_heliporter_helitransporter_plot_ravitaillement = &asnMission;

    const Position& pointDebarquement_ = pTarget_->GetTestParam_Point();
    const Position& positionRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPoint( pointDebarquement_, asnMission.point_debarquement );
    ASN_Tools::CopyPoint( positionRegroupement_, asnMission.position_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &pointDebarquement_;
    delete &positionRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement ());
    Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.position_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


