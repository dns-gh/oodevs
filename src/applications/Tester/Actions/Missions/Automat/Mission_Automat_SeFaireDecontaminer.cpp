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
#include "Mission_Automat_SeFaireDecontaminer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireDecontaminer::Mission_Automat_SeFaireDecontaminer( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_SeFaireDecontaminer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_SeFaireDecontaminer::~Mission_Automat_SeFaireDecontaminer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireDecontaminer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *new ASN1T_Mission_Automate_SeFaireDecontaminer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_se_faire_decontaminer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_se_faire_decontaminer = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_arrivee );
    ASN_Tools::CopyObjectKnowledge( pTarget_->GetTestParam_ObjectKnowledge(), asnMission.site_decontamination );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_SeFaireDecontaminer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_SeFaireDecontaminer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_se_faire_decontaminer );
    ASN1T_Mission_Automate_SeFaireDecontaminer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_se_faire_decontaminer;

    ASN_Tools::Delete( asnMission.zone_arrivee );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




