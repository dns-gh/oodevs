// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_RecueillirFormationAeromobile.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_RecueillirFormationAeromobile constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_RecueillirFormationAeromobile::Mission_Automat_ALAT_RecueillirFormationAeromobile( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_RecueillirFormationAeromobile", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_RecueillirFormationAeromobile destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_RecueillirFormationAeromobile::~Mission_Automat_ALAT_RecueillirFormationAeromobile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_RecueillirFormationAeromobile::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_RecueillirFormationAeromobile::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ALAT_RecueillirFormationAeromobile& asnMission = *new Mission_Automate_ALAT_RecueillirFormationAeromobile();

    asnMsg_.GetAsnMsg().mission().mission_automate_alat_recueillir_formation_aeromobile = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );

    delete &zone_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_RecueillirFormationAeromobile::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_RecueillirFormationAeromobile::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_alat_recueillir_formation_aeromobile ());
    Mission_Automate_ALAT_RecueillirFormationAeromobile& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_alat_recueillir_formation_aeromobile;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



