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
#include "Mission_Pawn_INF_Harceler.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Harceler::Mission_Pawn_INF_Harceler( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_INF_Harceler";
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

    ASN1T_Mission_Pion_INF_Harceler& asnMission = *new ASN1T_Mission_Pion_INF_Harceler();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_harceler;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_harceler = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.secteur );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.nature_objectif );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Harceler::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Harceler::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_harceler );
    ASN1T_Mission_Pion_INF_Harceler& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_harceler;

    ASN_Tools::Delete( asnMission.secteur );
    ASN_Tools::Delete( asnMission.point_regroupement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


