// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_FiltrerPopulations.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FiltrerPopulations constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FiltrerPopulations::Mission_Automat_FiltrerPopulations( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_FiltrerPopulations", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FiltrerPopulations destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_FiltrerPopulations::~Mission_Automat_FiltrerPopulations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FiltrerPopulations::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_FiltrerPopulations::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    Mission_Automate_FiltrerPopulations& asnMission = *new Mission_Automate_FiltrerPopulations();

    asnMsg_.GetAsnMsg().mission().mission_automate_filtrer_populations = &asnMission;

    const T_PositionVector& points_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPointList( points_, asnMission.points );

    delete &points_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_FiltrerPopulations::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_FiltrerPopulations::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_filtrer_populations ());
    Mission_Automate_FiltrerPopulations& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_filtrer_populations;

    ASN_Tools::Delete( asnMission.points );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}



