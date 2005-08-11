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
#include "Mission_Pawn_RENS_ROHUM_SExfiltrer.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_SExfiltrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_SExfiltrer::Mission_Pawn_RENS_ROHUM_SExfiltrer( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_RENS_ROHUM_SExfiltrer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_SExfiltrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROHUM_SExfiltrer::~Mission_Pawn_RENS_ROHUM_SExfiltrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_SExfiltrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_SExfiltrer::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_rohum_sexfiltrer = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_de_regroupement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROHUM_SExfiltrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROHUM_SExfiltrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_rohum_sexfiltrer );
    ASN1T_Mission_Pion_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_rohum_sexfiltrer;

    ASN_Tools::Delete( asnMission.point_de_regroupement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


