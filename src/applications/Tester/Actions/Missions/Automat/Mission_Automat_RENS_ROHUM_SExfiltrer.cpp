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
#include "Mission_Automat_RENS_ROHUM_SExfiltrer.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_SExfiltrer constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROHUM_SExfiltrer::Mission_Automat_RENS_ROHUM_SExfiltrer( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_RENS_ROHUM_SExfiltrer";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_SExfiltrer destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROHUM_SExfiltrer::~Mission_Automat_RENS_ROHUM_SExfiltrer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_SExfiltrer::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROHUM_SExfiltrer::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *new ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_rens_rohum_sexfiltrer = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_de_regroupement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_SExfiltrer::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROHUM_SExfiltrer::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_rens_rohum_sexfiltrer );
    ASN1T_Mission_Automate_RENS_ROHUM_SExfiltrer& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_rens_rohum_sexfiltrer;

    ASN_Tools::Delete( asnMission.zone_de_regroupement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




