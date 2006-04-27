// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RealiserTravauxSommairesAuProfitPopulation", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::~Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population = &asnMission;

    const T_GenObjectVector& travaux_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( travaux_, asnMission.travaux );

    delete &travaux_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserTravauxSommairesAuProfitPopulation::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population );
    ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population;

    ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




