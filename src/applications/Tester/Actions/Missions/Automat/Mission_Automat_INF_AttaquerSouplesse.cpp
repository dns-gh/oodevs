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
#include "Mission_Automat_INF_AttaquerSouplesse.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerSouplesse::Mission_Automat_INF_AttaquerSouplesse( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_INF_AttaquerSouplesse";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_INF_AttaquerSouplesse::~Mission_Automat_INF_AttaquerSouplesse()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerSouplesse::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *new ASN1T_Mission_Automate_INF_AttaquerSouplesse();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_inf_attaquer_souplesse;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_attaquer_souplesse = &asnMission;

    ASN_Tools::CopyPointList( pTarget_->GetTestParam_PointList(), asnMission.objectifs );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_INF_AttaquerSouplesse::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_INF_AttaquerSouplesse::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_inf_attaquer_souplesse );
    ASN1T_Mission_Automate_INF_AttaquerSouplesse& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_inf_attaquer_souplesse;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




