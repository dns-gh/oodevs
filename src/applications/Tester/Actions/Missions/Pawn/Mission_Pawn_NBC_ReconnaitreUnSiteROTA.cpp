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
#include "Mission_Pawn_NBC_ReconnaitreUnSiteROTA.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreUnSiteROTA constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreUnSiteROTA::Mission_Pawn_NBC_ReconnaitreUnSiteROTA( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_NBC_ReconnaitreUnSiteROTA";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreUnSiteROTA destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreUnSiteROTA::~Mission_Pawn_NBC_ReconnaitreUnSiteROTA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreUnSiteROTA::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreUnSiteROTA::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_un_site_rota = &asnMission;

    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_ROTA );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreUnSiteROTA::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreUnSiteROTA::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_nbc_reconnaitre_un_site_rota );
    ASN1T_Mission_Pion_NBC_ReconnaitreUnSiteROTA& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_un_site_rota;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


