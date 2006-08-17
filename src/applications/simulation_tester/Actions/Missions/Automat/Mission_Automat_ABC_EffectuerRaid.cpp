// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_ABC_EffectuerRaid.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EffectuerRaid constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EffectuerRaid::Mission_Automat_ABC_EffectuerRaid( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_ABC_EffectuerRaid", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EffectuerRaid destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EffectuerRaid::~Mission_Automat_ABC_EffectuerRaid()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EffectuerRaid::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EffectuerRaid::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_EffectuerRaid& asnMission = *new ASN1T_Mission_Automate_ABC_EffectuerRaid();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_effectuer_raid;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_effectuer_raid = &asnMission;




}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EffectuerRaid::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EffectuerRaid::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_effectuer_raid );
    ASN1T_Mission_Automate_ABC_EffectuerRaid& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_effectuer_raid;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




