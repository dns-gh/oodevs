// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Mission_Pawn_ALAT_DetruireNeutraliserDansZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Mission_Pawn_ALAT_DetruireNeutraliserDansZone( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ALAT_DetruireNeutraliserDansZone";
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

    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *new ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_detruire_neutraliser_dans_zone = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_dislocation );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.cibles_prioritaires );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.ambiance_mission );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.neutraliser );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_DetruireNeutraliserDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_detruire_neutraliser_dans_zone );
    ASN1T_Mission_Pion_ALAT_DetruireNeutraliserDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_detruire_neutraliser_dans_zone;

    ASN_Tools::Delete( asnMission.zone );
    ASN_Tools::Delete( asnMission.point_dislocation );
    ASN_Tools::Delete( asnMission.point_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


