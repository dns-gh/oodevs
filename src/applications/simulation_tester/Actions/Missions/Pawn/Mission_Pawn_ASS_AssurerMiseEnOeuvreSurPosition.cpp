// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_AssurerMiseEnOeuvreSurPosition", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::~Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *new Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition();

    asnMsg_.GetAsnMsg().mission().mission_pion_ass_assurer_mise_en_oeuvre_sur_position = &asnMission;

    const Position& positionImplantation_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionImplantation_, asnMission.position_implantation );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.presence_contre_batterie );

    delete &positionImplantation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AssurerMiseEnOeuvreSurPosition::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_ass_assurer_mise_en_oeuvre_sur_position ());
    Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_ass_assurer_mise_en_oeuvre_sur_position;

    ASN_Tools::Delete( asnMission.position_implantation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

