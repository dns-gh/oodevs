// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASY_CommettreAttentatContreForcesArmees.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreForcesArmees constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::Mission_Pawn_ASY_CommettreAttentatContreForcesArmees( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_ASY_CommettreAttentatContreForcesArmees", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreForcesArmees destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::~Mission_Pawn_ASY_CommettreAttentatContreForcesArmees()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *new Mission_Pion_ASY_CommettreAttentatContreForcesArmees();

    asnMsg_.GetAsnMsg().mission().mission_pion_asy_commettre_attentatContreForcesArmees = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointRepli_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointRepli_, asnMission.point_repli );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.type_attentat_forces_armees );
    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.unite_cible );

    delete &itineraire_;
    delete &pointRepli_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentatContreForcesArmees::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_asy_commettre_attentatContreForcesArmees ());
    Mission_Pion_ASY_CommettreAttentatContreForcesArmees& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_asy_commettre_attentatContreForcesArmees;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


