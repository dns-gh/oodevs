// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASS_AcquerirLanceursEni.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AcquerirLanceursEni constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AcquerirLanceursEni::Mission_Pawn_ASS_AcquerirLanceursEni( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_AcquerirLanceursEni", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AcquerirLanceursEni destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_AcquerirLanceursEni::~Mission_Pawn_ASS_AcquerirLanceursEni()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AcquerirLanceursEni::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AcquerirLanceursEni::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *new ASN1T_Mission_Pion_ASS_AcquerirLanceursEni();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_acquerir_lanceurs_eni = &asnMission;

    const T_LocationVector& zonesAObserver_ = pTarget_->GetTestParam_PolygonList();

    ASN_Tools::CopyPolygonList( zonesAObserver_, asnMission.zones_a_observer );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.zone_implantation );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.menance_roem );

    delete &zonesAObserver_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_AcquerirLanceursEni::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_AcquerirLanceursEni::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni );
    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_acquerir_lanceurs_eni;

    ASN_Tools::Delete( asnMission.zones_a_observer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


