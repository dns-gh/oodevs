// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_Harceler.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Harceler::Mission_Pawn_INF_Harceler( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_Harceler", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Harceler::~Mission_Pawn_INF_Harceler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Harceler::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_Harceler& asnMission = *new Mission_Pion_INF_Harceler();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_harceler = &asnMission;

    const Location& secteur_ = pTarget_->GetTestParam_Polygon();
    const Position& pointRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( secteur_, asnMission.secteur );
    ASN_Tools::CopyPoint( pointRegroupement_, asnMission.point_regroupement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.nature_objectif );

    delete &secteur_;
    delete &pointRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Harceler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_harceler ());
    Mission_Pion_INF_Harceler& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_harceler;

    ASN_Tools::Delete( asnMission.secteur );
    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


