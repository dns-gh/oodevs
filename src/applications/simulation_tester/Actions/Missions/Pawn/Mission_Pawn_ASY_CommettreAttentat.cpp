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

#include "simulation_tester_pch.h"
#include "Mission_Pawn_ASY_CommettreAttentat.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentat constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentat::Mission_Pawn_ASY_CommettreAttentat( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ASY_CommettreAttentat";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentat destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASY_CommettreAttentat::~Mission_Pawn_ASY_CommettreAttentat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentat::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentat::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASY_CommettreAttentat& asnMission = *new ASN1T_Mission_Pion_ASY_CommettreAttentat();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asy_commettre_attentat;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentat = &asnMission;

    const Path& itineraire_ = pTarget_->GetTestParam_Path();
    const Position& pointRepli_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPath( itineraire_, asnMission.itineraire );
    ASN_Tools::CopyPoint( pointRepli_, asnMission.point_repli );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 2 ), asnMission.cible_attentat );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.type_attentat_forces_armees );
    ASN_Tools::CopyAgentKnowledge( pTarget_->GetTestParam_AgentKnowledge(), asnMission.unite_cible );
    ASN_Tools::CopyPopulationKnowledge( pTarget_->GetTestParam_PopulationKnowledge(), asnMission.population_cible );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.installation_cible );

    delete &itineraire_;
    delete &pointRepli_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASY_CommettreAttentat::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASY_CommettreAttentat::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asy_commettre_attentat );
    ASN1T_Mission_Pion_ASY_CommettreAttentat& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asy_commettre_attentat;

    ASN_Tools::Delete( asnMission.itineraire );
    ASN_Tools::Delete( asnMission.point_repli );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


