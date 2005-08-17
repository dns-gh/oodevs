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
#include "Mission_Automat_ABC_ArmerPointDeControle.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ArmerPointDeControle constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ArmerPointDeControle::Mission_Automat_ABC_ArmerPointDeControle( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_ArmerPointDeControle";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ArmerPointDeControle destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_ArmerPointDeControle::~Mission_Automat_ABC_ArmerPointDeControle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ArmerPointDeControle::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ArmerPointDeControle::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Automate_ABC_ArmerPointDeControle();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_armer_point_de_controle;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_armer_point_de_controle = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_de_controle );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_ArmerPointDeControle::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_ArmerPointDeControle::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_armer_point_de_controle );
    ASN1T_Mission_Automate_ABC_ArmerPointDeControle& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_armer_point_de_controle;

    ASN_Tools::Delete( asnMission.zone_de_controle );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




