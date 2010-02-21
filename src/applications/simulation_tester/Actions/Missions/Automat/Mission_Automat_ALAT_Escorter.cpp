// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_Escorter.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Escorter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Escorter::Mission_Automat_ALAT_Escorter( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_Escorter", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Escorter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Escorter::~Mission_Automat_ALAT_Escorter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Escorter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Escorter::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ALAT_Escorter& asnMission = *new Mission_Automate_ALAT_Escorter();

    asnMsg_.GetAsnMsg().mission().mission_automate_alat_escorter = &asnMission;

    const T_IdVector& unitesAEscorter_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyAgentList( unitesAEscorter_, asnMission.unites_a_escorter );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &unitesAEscorter_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Escorter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Escorter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_alat_escorter ());
    Mission_Automate_ALAT_Escorter& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_alat_escorter;

    ASN_Tools::Delete( asnMission.unites_a_escorter );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




