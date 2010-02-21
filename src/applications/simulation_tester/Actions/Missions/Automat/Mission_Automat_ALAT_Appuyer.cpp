// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_Appuyer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Appuyer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Appuyer::Mission_Automat_ALAT_Appuyer( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_Appuyer", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Appuyer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Appuyer::~Mission_Automat_ALAT_Appuyer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Appuyer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Appuyer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ALAT_Appuyer& asnMission = *new Mission_Automate_ALAT_Appuyer();

    asnMsg_.GetAsnMsg().mission().mission_automate_alat_appuyer = &asnMission;

    const T_IdVector& unitesAAppuyer_ = pTarget_->GetTestParam_AgentList();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();
    const Position& pointDislocation_ = pTarget_->GetTestParam_Point();
    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyAgentList( unitesAAppuyer_, asnMission.unites_a_appuyer );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyPoint( pointDislocation_, asnMission.point_dislocation );
    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &unitesAAppuyer_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;
    delete &pointDislocation_;
    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Appuyer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Appuyer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_alat_appuyer ());
    Mission_Automate_ALAT_Appuyer& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_alat_appuyer;

    ASN_Tools::Delete( asnMission.unites_a_appuyer );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );
    ASN_Tools::Delete( asnMission.point_dislocation );
    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




