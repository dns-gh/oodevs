// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASY_CommettreAttentatContreInstallation.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreInstallation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContreInstallation::Mission_Pawn_ASY_CommettreAttentatContreInstallation( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASY_CommettreAttentatContreInstallation", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreInstallation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContreInstallation::~Mission_Pawn_ASY_CommettreAttentatContreInstallation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreInstallation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContreInstallation::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreInstallation = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointRepli_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointRepli_, asnMission.point_repli );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.installation_cible );

    delete &itineraire_;
    delete &pointRepli_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreInstallation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContreInstallation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentatContreInstallation );
    ASN1T_Mission_Pion_ASY_CommettreAttentatContreInstallation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentatContreInstallation;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


