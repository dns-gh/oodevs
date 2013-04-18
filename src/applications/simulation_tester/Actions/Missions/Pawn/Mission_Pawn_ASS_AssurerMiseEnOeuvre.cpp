// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASS_AssurerMiseEnOeuvre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AssurerMiseEnOeuvre::Mission_Pawn_ASS_AssurerMiseEnOeuvre( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_AssurerMiseEnOeuvre", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AssurerMiseEnOeuvre::~Mission_Pawn_ASS_AssurerMiseEnOeuvre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AssurerMiseEnOeuvre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *new Mission_Pion_ASS_AssurerMiseEnOeuvre();

    asnMsg_.GetAsnMsg().mission().mission_pion_ass_assurer_mise_en_oeuvre = &asnMission;


    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.zone_implantation );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.presence_contre_batterie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AssurerMiseEnOeuvre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AssurerMiseEnOeuvre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_ass_assurer_mise_en_oeuvre ());
    Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_ass_assurer_mise_en_oeuvre;


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

