// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_AppuyerDirectAuContact.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AppuyerDirectAuContact constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_AppuyerDirectAuContact::Mission_Pawn_ALAT_AppuyerDirectAuContact( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_AppuyerDirectAuContact", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AppuyerDirectAuContact destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_AppuyerDirectAuContact::~Mission_Pawn_ALAT_AppuyerDirectAuContact()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AppuyerDirectAuContact::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_AppuyerDirectAuContact::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ALAT_AppuyerDirectAuContact& asnMission = *new Mission_Pion_ALAT_AppuyerDirectAuContact();

    asnMsg_.GetAsnMsg().mission().mission_pion_alat_appuyer_direct_au_contact = &asnMission;

    const T_IdVector& unitesAAppuyer_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyAgentList( unitesAAppuyer_, asnMission.unites_a_appuyer );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &unitesAAppuyer_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_AppuyerDirectAuContact::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_AppuyerDirectAuContact::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_alat_appuyer_direct_au_contact ());
    Mission_Pion_ALAT_AppuyerDirectAuContact& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_alat_appuyer_direct_au_contact;

    ASN_Tools::Delete( asnMission.unites_a_appuyer );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

