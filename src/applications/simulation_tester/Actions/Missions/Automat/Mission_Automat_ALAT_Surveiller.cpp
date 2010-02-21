// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_Surveiller.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Surveiller::Mission_Automat_ALAT_Surveiller( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_Surveiller", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_Surveiller::~Mission_Automat_ALAT_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_ALAT_Surveiller& asnMission = *new Mission_Automate_ALAT_Surveiller();

    asnMsg_.GetAsnMsg().mission().mission_automate_alat_surveiller = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();
    const Position& pointLogistique_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyPoint( pointLogistique_, asnMission.point_logistique );

    delete &zone_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;
    delete &pointLogistique_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_alat_surveiller ());
    Mission_Automate_ALAT_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_alat_surveiller;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );
    ASN_Tools::Delete( asnMission.point_logistique );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




