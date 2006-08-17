// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_NBC_ReconnaitreDesSitesROTA.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreDesSitesROTA constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreDesSitesROTA::Mission_Automat_NBC_ReconnaitreDesSitesROTA( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_NBC_ReconnaitreDesSitesROTA", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreDesSitesROTA destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ReconnaitreDesSitesROTA::~Mission_Automat_NBC_ReconnaitreDesSitesROTA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreDesSitesROTA::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreDesSitesROTA::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *new ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_des_sites_rota = &asnMission;

    const T_IdVector& sitesROTA_ = pTarget_->GetTestParam_ObjectKnowledgeList();

    ASN_Tools::CopyObjectKnowledgeList( sitesROTA_, asnMission.sites_ROTA );

    delete &sitesROTA_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ReconnaitreDesSitesROTA::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ReconnaitreDesSitesROTA::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_nbc_reconnaitre_des_sites_rota );
    ASN1T_Mission_Automate_NBC_ReconnaitreDesSitesROTA& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_reconnaitre_des_sites_rota;

    ASN_Tools::Delete( asnMission.sites_ROTA );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




