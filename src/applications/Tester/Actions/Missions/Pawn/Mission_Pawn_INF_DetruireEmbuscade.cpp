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
#include "Mission_Pawn_INF_DetruireEmbuscade.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireEmbuscade constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_DetruireEmbuscade::Mission_Pawn_INF_DetruireEmbuscade( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_DetruireEmbuscade";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireEmbuscade destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_DetruireEmbuscade::~Mission_Pawn_INF_DetruireEmbuscade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireEmbuscade::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_DetruireEmbuscade::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *new ASN1T_Mission_Pion_INF_DetruireEmbuscade();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_embuscade;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_detruire_embuscade = &asnMission;

    const Position& positionEmbuscade_ = pTarget_->GetTestParam_Point();
    const Position& positionRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionEmbuscade_, asnMission.position_embuscade );
    ASN_Tools::CopyPoint( positionRegroupement_, asnMission.position_regroupement );
    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.ennemi );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 37 ), asnMission.nature_obstacle );

    delete &positionEmbuscade_;
    delete &positionRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireEmbuscade::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_DetruireEmbuscade::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_detruire_embuscade );
    ASN1T_Mission_Pion_INF_DetruireEmbuscade& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_detruire_embuscade;

    ASN_Tools::Delete( asnMission.position_embuscade );
    ASN_Tools::Delete( asnMission.position_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


