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
#include "Mission_Automat_ABC_ReleverEnDepassant.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReleverEnDepassant constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ReleverEnDepassant::Mission_Automat_ABC_ReleverEnDepassant( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_ReleverEnDepassant";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReleverEnDepassant destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ReleverEnDepassant::~Mission_Automat_ABC_ReleverEnDepassant()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReleverEnDepassant::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ReleverEnDepassant::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *new ASN1T_Mission_Automate_ABC_ReleverEnDepassant();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_relever_en_depassant;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_relever_en_depassant = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.compagnie );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ReleverEnDepassant::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ReleverEnDepassant::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_relever_en_depassant );
    ASN1T_Mission_Automate_ABC_ReleverEnDepassant& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_relever_en_depassant;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




