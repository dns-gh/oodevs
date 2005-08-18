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
#include "Mission_Pawn_NBC_ReconnaitreZone.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreZone::Mission_Pawn_NBC_ReconnaitreZone( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_NBC_ReconnaitreZone";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_NBC_ReconnaitreZone::~Mission_Pawn_NBC_ReconnaitreZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreZone::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_NBC_ReconnaitreZone& asnMission = *new ASN1T_Mission_Pion_NBC_ReconnaitreZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_nbc_reconnaitre_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_zone = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_NBC_ReconnaitreZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_NBC_ReconnaitreZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_nbc_reconnaitre_zone );
    ASN1T_Mission_Pion_NBC_ReconnaitreZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_nbc_reconnaitre_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


