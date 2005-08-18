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
#include "Mission_Pawn_ASA_DefendreSiteAPartirPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSiteAPartirPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreSiteAPartirPosition::Mission_Pawn_ASA_DefendreSiteAPartirPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ASA_DefendreSiteAPartirPosition";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSiteAPartirPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_DefendreSiteAPartirPosition::~Mission_Pawn_ASA_DefendreSiteAPartirPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSiteAPartirPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreSiteAPartirPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_site_a_partir_position = &asnMission;

    const Location& site_ = pTarget_->GetTestParam_Polygon();
    const Position& pointDeDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( site_, asnMission.site );
    ASN_Tools::CopyPoint( pointDeDeploiement_, asnMission.point_de_deploiement );

    delete &site_;
    delete &pointDeDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_DefendreSiteAPartirPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_DefendreSiteAPartirPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_defendre_site_a_partir_position );
    ASN1T_Mission_Pion_ASA_DefendreSiteAPartirPosition& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_defendre_site_a_partir_position;

    ASN_Tools::Delete( asnMission.site );
    ASN_Tools::Delete( asnMission.point_de_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


