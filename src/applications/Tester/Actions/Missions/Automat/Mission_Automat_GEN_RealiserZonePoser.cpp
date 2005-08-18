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
#include "Mission_Automat_GEN_RealiserZonePoser.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserZonePoser::Mission_Automat_GEN_RealiserZonePoser( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_RealiserZonePoser";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserZonePoser::~Mission_Automat_GEN_RealiserZonePoser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserZonePoser::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserZonePoser& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserZonePoser();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_zone_poser;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser = &asnMission;


    ASN_Tools::CopyGenObject( pTarget_->GetTestParam_GenObject(), asnMission.zone );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserZonePoser::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserZonePoser::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_zone_poser );
    ASN1T_Mission_Automate_GEN_RealiserZonePoser& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_zone_poser;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




