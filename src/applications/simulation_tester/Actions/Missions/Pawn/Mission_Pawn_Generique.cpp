// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_Generique.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Generique constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Generique::Mission_Pawn_Generique( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_Generique", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Generique destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_Generique::~Mission_Pawn_Generique()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Generique::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_Generique::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_Generique& asnMission = *new ASN1T_Mission_Pion_Generique();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_generique;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_generique = &asnMission;

    const T_LocationVector& zones_ = pTarget_->GetTestParam_PolygonList();
    const T_PositionVector& points_ = pTarget_->GetTestParam_PointList();
    const T_PathVector& itineraires_ = pTarget_->GetTestParam_PathList();
    const T_IdVector& unites_ = pTarget_->GetTestParam_AgentList();
    const T_IdVector& automates_ = pTarget_->GetTestParam_AutomateList();
    const T_IdVector& connaissancesAgents_ = pTarget_->GetTestParam_AgentKnowledgeList();
    const T_IdVector& connaissancesObjets_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyPolygonList( zones_, asnMission.zones );
    ASN_Tools::CopyPointList( points_, asnMission.points );
    ASN_Tools::CopyPathList( itineraires_, asnMission.itineraires );
    ASN_Tools::CopyAgentList( unites_, asnMission.unites );
    ASN_Tools::CopyAutomateList( automates_, asnMission.automates );
    ASN_Tools::CopyAgentKnowledgeList( connaissancesAgents_, asnMission.connaissances_agents );
    ASN_Tools::CopyObjectKnowledgeList( connaissancesObjets_, asnMission.connaissances_objets );

    delete &zones_;
    delete &points_;
    delete &itineraires_;
    delete &unites_;
    delete &automates_;
    delete &connaissancesAgents_;
    delete &connaissancesObjets_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Generique::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_Generique::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_generique );
    ASN1T_Mission_Pion_Generique& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_generique;

    ASN_Tools::Delete( asnMission.zones );
    ASN_Tools::Delete( asnMission.points );
    ASN_Tools::Delete( asnMission.itineraires );
    ASN_Tools::Delete( asnMission.unites );
    ASN_Tools::Delete( asnMission.automates );
    ASN_Tools::Delete( asnMission.connaissances_agents );
    ASN_Tools::Delete( asnMission.connaissances_objets );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


