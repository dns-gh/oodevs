// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_ASS_EOP_AcquerirObjectifs.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EOP_AcquerirObjectifs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_EOP_AcquerirObjectifs::Mission_Pawn_ASS_EOP_AcquerirObjectifs( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASS_EOP_AcquerirObjectifs", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EOP_AcquerirObjectifs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_EOP_AcquerirObjectifs::~Mission_Pawn_ASS_EOP_AcquerirObjectifs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EOP_AcquerirObjectifs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_EOP_AcquerirObjectifs::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_eop_acquerir_objectifs = &asnMission;

    const Location& zoneAObserver_ = pTarget_->GetTestParam_Polygon();
    const Position& positionInstallation_ = pTarget_->GetTestParam_Point();
    const Position& positionDebarquement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPolygon( zoneAObserver_, asnMission.zone_a_observer );
    ASN_Tools::CopyPoint( positionInstallation_, asnMission.position_installation );
    ASN_Tools::CopyPoint( positionDebarquement_, asnMission.position_debarquement );

    delete &zoneAObserver_;
    delete &positionInstallation_;
    delete &positionDebarquement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EOP_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_EOP_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_eop_acquerir_objectifs );
    ASN1T_Mission_Pion_ASS_EOP_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_eop_acquerir_objectifs;

    ASN_Tools::Delete( asnMission.zone_a_observer );
    ASN_Tools::Delete( asnMission.position_installation );
    ASN_Tools::Delete( asnMission.position_debarquement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


