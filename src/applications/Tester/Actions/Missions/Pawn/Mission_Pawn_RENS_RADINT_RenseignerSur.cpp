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
#include "Mission_Pawn_RENS_RADINT_RenseignerSur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_RenseignerSur::Mission_Pawn_RENS_RADINT_RenseignerSur( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_RENS_RADINT_RenseignerSur";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_RADINT_RenseignerSur::~Mission_Pawn_RENS_RADINT_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_radint_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur = &asnMission;

    ASN_Tools::CopyPolygonList( pTarget_->GetTestParam_PolygonList(), asnMission.zones_de_recherche );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_RADINT_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_RADINT_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_radint_renseigner_sur );
    ASN1T_Mission_Pion_RENS_RADINT_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_radint_renseigner_sur;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


