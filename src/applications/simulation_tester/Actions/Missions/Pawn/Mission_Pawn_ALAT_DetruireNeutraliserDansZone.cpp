// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ALAT_DetruireNeutraliserDansZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Mission_Pawn_ALAT_DetruireNeutraliserDansZone( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ALAT_DetruireNeutraliserDansZone", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_DetruireNeutraliserDansZone::~Mission_Pawn_ALAT_DetruireNeutraliserDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *new Mission_Pion_ALAT_DetruireNeutraliserDansZone();

    asnMsg_.GetAsnMsg().mission().mission_pion_alat_detruire_neutraliser_dans_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();
    const Position& pointDislocation_ = pTarget_->GetTestParam_Point();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();
    const T_IdVector& plotsRavitaillement_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );
    ASN_Tools::CopyPoint( pointDislocation_, asnMission.point_dislocation );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( plotsRavitaillement_, asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.neutraliser );

    delete &zone_;
    delete &pointDislocation_;
    delete &pointRegroupement_;
    delete &plotsRavitaillement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_alat_detruire_neutraliser_dans_zone ());
    Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_alat_detruire_neutraliser_dans_zone;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_dislocation );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

