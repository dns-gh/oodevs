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
#include "Mission_Pawn_SecuriserZoneContreMenacesAsymetriques.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContreMenacesAsymetriques constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::Mission_Pawn_SecuriserZoneContreMenacesAsymetriques( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_SecuriserZoneContreMenacesAsymetriques";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContreMenacesAsymetriques destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::~Mission_Pawn_SecuriserZoneContreMenacesAsymetriques()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques& asnMission = *new ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_securiser_zone_contre_menaces_asymetriques;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_menaces_asymetriques = &asnMission;

    const Location& zone_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zone_, asnMission.zone );

    delete &zone_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_SecuriserZoneContreMenacesAsymetriques::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_securiser_zone_contre_menaces_asymetriques );
    ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_securiser_zone_contre_menaces_asymetriques;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


