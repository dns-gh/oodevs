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
#include "Mission_Pawn_GEN_RealiserTravauxProtection.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxProtection::Mission_Pawn_GEN_RealiserTravauxProtection( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_GEN_RealiserTravauxProtection";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserTravauxProtection::~Mission_Pawn_GEN_RealiserTravauxProtection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxProtection::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection = &asnMission;

    const Position& posObjet_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObjet_, asnMission.pos_objet );
    ASN_Tools::CopyID( pTarget_->GetTestParam_ID(), asnMission.oid_objet_planifie );

    delete &posObjet_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserTravauxProtection::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserTravauxProtection::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_protection );
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_travaux_protection;

    ASN_Tools::Delete( asnMission.pos_objet );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


