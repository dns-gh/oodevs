// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_MILICE_DetruireEmbuscade.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_DetruireEmbuscade constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_DetruireEmbuscade::Mission_Pawn_MILICE_DetruireEmbuscade( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_MILICE_DetruireEmbuscade", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_DetruireEmbuscade destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_MILICE_DetruireEmbuscade::~Mission_Pawn_MILICE_DetruireEmbuscade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_DetruireEmbuscade::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_DetruireEmbuscade::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_MILICE_DetruireEmbuscade& asnMission = *new Mission_Pion_MILICE_DetruireEmbuscade();

    asnMsg_.GetAsnMsg().mission().mission_pion_milice_detruire_embuscade = &asnMission;

    const Position& positionEmbuscade_ = pTarget_->GetTestParam_Point();
    const Position& positionRegroupement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionEmbuscade_, asnMission.position_embuscade );
    ASN_Tools::CopyPoint( positionRegroupement_, asnMission.position_regroupement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 38 ), asnMission.nature_obstacle );

    delete &positionEmbuscade_;
    delete &positionRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_MILICE_DetruireEmbuscade::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_MILICE_DetruireEmbuscade::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_milice_detruire_embuscade ());
    Mission_Pion_MILICE_DetruireEmbuscade& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_milice_detruire_embuscade;

    ASN_Tools::Delete( asnMission.position_embuscade );
    ASN_Tools::Delete( asnMission.position_regroupement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

