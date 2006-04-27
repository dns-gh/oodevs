// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_ReconnaitreDansLaProfondeur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::~Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *new ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur = &asnMission;

    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();
    const Position& pointDislocation_ = pTarget_->GetTestParam_Point();
    const Location& zone_ = pTarget_->GetTestParam_Location();

    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );
    ASN_Tools::CopyPoint( pointDislocation_, asnMission.point_dislocation );
    ASN_Tools::CopyLocation( zone_, asnMission.zone );

    delete &pointRegroupement_;
    delete &plotsRavitaillement_;
    delete &pointDislocation_;
    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_reconnaitre_dans_la_profondeur );
    ASN1T_Mission_Pion_ALAT_ReconnaitreDansLaProfondeur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_reconnaitre_dans_la_profondeur;

    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );
    ASN_Tools::Delete( asnMission.point_dislocation );
    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


