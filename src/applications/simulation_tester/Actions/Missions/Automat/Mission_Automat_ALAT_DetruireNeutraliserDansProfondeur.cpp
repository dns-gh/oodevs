// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ALAT_DetruireNeutraliserDansProfondeur", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::~Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur& asnMission = *new ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_profondeur = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const Position& pointDislocation_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyPoint( pointDislocation_, asnMission.point_dislocation );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.neutraliser );

    delete &zone_;
    delete &pointRegroupement_;
    delete &pointDislocation_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ALAT_DetruireNeutraliserDansProfondeur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_alat_detruire_neutraliser_dans_profondeur );
    ASN1T_Mission_Automate_ALAT_DetruireNeutraliserDansProfondeur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_alat_detruire_neutraliser_dans_profondeur;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.point_dislocation );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




