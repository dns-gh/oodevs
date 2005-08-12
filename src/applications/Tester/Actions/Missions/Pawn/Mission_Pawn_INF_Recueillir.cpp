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
#include "Mission_Pawn_INF_Recueillir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Recueillir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Recueillir::Mission_Pawn_INF_Recueillir( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_Recueillir";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Recueillir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_Recueillir::~Mission_Pawn_INF_Recueillir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Recueillir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Recueillir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *new ASN1T_Mission_Pion_INF_Recueillir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_recueillir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_recueillir = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_installation );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.pia );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_Recueillir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_Recueillir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_recueillir );
    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_recueillir;

    ASN_Tools::Delete( asnMission.position_installation );
    ASN_Tools::Delete( asnMission.pia );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


