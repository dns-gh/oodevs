// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_RENS_ROHUM_OrienterGuider.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_OrienterGuider constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROHUM_OrienterGuider::Mission_Automat_RENS_ROHUM_OrienterGuider( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_RENS_ROHUM_OrienterGuider", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_OrienterGuider destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROHUM_OrienterGuider::~Mission_Automat_RENS_ROHUM_OrienterGuider()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_OrienterGuider::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROHUM_OrienterGuider::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *new Mission_Automate_RENS_ROHUM_OrienterGuider();

    asnMsg_.GetAsnMsg().mission().mission_automate_rens_rohum_orienter_guider = &asnMission;


    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.ami );


}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROHUM_OrienterGuider::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROHUM_OrienterGuider::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_rens_rohum_orienter_guider ());
    Mission_Automate_RENS_ROHUM_OrienterGuider& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_rens_rohum_orienter_guider;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



