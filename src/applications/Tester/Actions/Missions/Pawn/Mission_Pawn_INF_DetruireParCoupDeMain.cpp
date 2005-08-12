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
#include "Mission_Pawn_INF_DetruireParCoupDeMain.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireParCoupDeMain constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_DetruireParCoupDeMain::Mission_Pawn_INF_DetruireParCoupDeMain( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_DetruireParCoupDeMain";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireParCoupDeMain destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_DetruireParCoupDeMain::~Mission_Pawn_INF_DetruireParCoupDeMain()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireParCoupDeMain::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_DetruireParCoupDeMain::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *new ASN1T_Mission_Pion_INF_DetruireParCoupDeMain();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_detruire_par_coup_de_main = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.objectif );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_regroupement );
    ASN_Tools::CopyGDH( pTarget_->GetTestParam_GDH(), asnMission.heure_destruction );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.nature_destruction );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_DetruireParCoupDeMain::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_DetruireParCoupDeMain::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main );
    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_detruire_par_coup_de_main;

    ASN_Tools::Delete( asnMission.objectif );
    ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


