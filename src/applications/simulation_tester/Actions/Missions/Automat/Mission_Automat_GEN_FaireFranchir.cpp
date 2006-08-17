// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_FaireFranchir.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FaireFranchir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_FaireFranchir::Mission_Automat_GEN_FaireFranchir( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_FaireFranchir", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FaireFranchir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_FaireFranchir::~Mission_Automat_GEN_FaireFranchir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FaireFranchir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_FaireFranchir::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *new ASN1T_Mission_Automate_GEN_FaireFranchir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_faire_franchir;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_faire_franchir = &asnMission;

    const T_IdVector& sitesDeFranchissement_ = pTarget_->GetTestParam_ObjectKnowledgeList();
    const Location& zoneRegroupement_ = pTarget_->GetTestParam_Location();

    ASN_Tools::CopyObjectKnowledgeList( sitesDeFranchissement_, asnMission.sites_de_franchissement );
    ASN_Tools::CopyLocation( zoneRegroupement_, asnMission.zone_regroupement );

    delete &sitesDeFranchissement_;
    delete &zoneRegroupement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_FaireFranchir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_FaireFranchir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_faire_franchir );
    ASN1T_Mission_Automate_GEN_FaireFranchir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_faire_franchir;

    ASN_Tools::Delete( asnMission.sites_de_franchissement );
    ASN_Tools::Delete( asnMission.zone_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




