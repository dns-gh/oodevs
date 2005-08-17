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
#include "Mission_Automat_Generique.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Generique constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Generique::Mission_Automat_Generique( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_Generique";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Generique destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_Generique::~Mission_Automat_Generique()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Generique::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_Generique::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_Generique& asnMission = *new ASN1T_Mission_Automate_Generique();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_generique;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_generique = &asnMission;

    ASN_Tools::CopyPolygonList( pTarget_->GetTestParam_PolygonList(), asnMission.zones );
    ASN_Tools::CopyPointList( pTarget_->GetTestParam_PointList(), asnMission.points );
    ASN_Tools::CopyPathList( pTarget_->GetTestParam_PathList(), asnMission.itineraires );
    ASN_Tools::CopyAgentList( pTarget_->GetTestParam_AgentList(), asnMission.unites );
    ASN_Tools::CopyAutomateList( pTarget_->GetTestParam_AutomateList(), asnMission.automates );
    ASN_Tools::CopyAgentKnowledgeList( pTarget_->GetTestParam_AgentKnowledgeList(), asnMission.connaissances_agents );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.connaissances_objets );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Generique::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_Generique::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_generique );
    ASN1T_Mission_Automate_Generique& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_generique;

    ASN_Tools::Delete( asnMission.connaissances_agents );
    ASN_Tools::Delete( asnMission.connaissances_objets );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




