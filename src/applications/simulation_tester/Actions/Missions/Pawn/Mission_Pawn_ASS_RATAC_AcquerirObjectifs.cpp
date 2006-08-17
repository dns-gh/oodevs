// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASS_RATAC_AcquerirObjectifs.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_RATAC_AcquerirObjectifs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_RATAC_AcquerirObjectifs::Mission_Pawn_ASS_RATAC_AcquerirObjectifs( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_RATAC_AcquerirObjectifs", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_RATAC_AcquerirObjectifs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_RATAC_AcquerirObjectifs::~Mission_Pawn_ASS_RATAC_AcquerirObjectifs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_RATAC_AcquerirObjectifs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_RATAC_AcquerirObjectifs::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_ratac_acquerir_objectifs = &asnMission;

    const Location& zoneAObserver_ = pTarget_->GetTestParam_Polygon();
    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( zoneAObserver_, asnMission.zone_a_observer );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.categories );
    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &zoneAObserver_;
    delete &positionDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_RATAC_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_RATAC_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_ratac_acquerir_objectifs );
    ASN1T_Mission_Pion_ASS_RATAC_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_ratac_acquerir_objectifs;

    ASN_Tools::Delete( asnMission.zone_a_observer );
    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


