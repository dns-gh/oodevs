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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_FaireFranchir";
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

    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.sites_de_franchissement );
    ASN_Tools::CopyLocation( pTarget_->GetTestParam_Location(), asnMission.zone_regroupement );

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




