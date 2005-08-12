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
#include "Mission_Pawn_ABC_ReconnaitreAxe.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitreAxe constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReconnaitreAxe::Mission_Pawn_ABC_ReconnaitreAxe( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ABC_ReconnaitreAxe";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitreAxe destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_ReconnaitreAxe::~Mission_Pawn_ABC_ReconnaitreAxe()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitreAxe::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReconnaitreAxe::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitreAxe();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_reconnaitre_axe;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_axe = &asnMission;

    ASN_Tools::CopyPath( pTarget_->GetTestParam_Path(), asnMission.itineraire );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_ReconnaitreAxe::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_ReconnaitreAxe::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_reconnaitre_axe );
    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_reconnaitre_axe;

    ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


