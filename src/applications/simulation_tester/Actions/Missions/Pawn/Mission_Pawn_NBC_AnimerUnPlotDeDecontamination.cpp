// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_NBC_AnimerUnPlotDeDecontamination.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_AnimerUnPlotDeDecontamination constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::Mission_Pawn_NBC_AnimerUnPlotDeDecontamination( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_NBC_AnimerUnPlotDeDecontamination", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_AnimerUnPlotDeDecontamination destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::~Mission_Pawn_NBC_AnimerUnPlotDeDecontamination()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *new Mission_Pion_NBC_AnimerUnPlotDeDecontamination();

    asnMsg_.GetAsnMsg().mission().mission_pion_nbc_animer_un_plot_de_decontamination = &asnMission;

    const Position& positionSiteDecontamination_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionSiteDecontamination_, asnMission.position_site_decontamination );

    delete &positionSiteDecontamination_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_AnimerUnPlotDeDecontamination::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_nbc_animer_un_plot_de_decontamination ());
    Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_nbc_animer_un_plot_de_decontamination;

    ASN_Tools::Delete( asnMission.position_site_decontamination );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


