// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Automat_GEN_RealiserCampPrisonniers.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampPrisonniers constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserCampPrisonniers::Mission_Automat_GEN_RealiserCampPrisonniers( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RealiserCampPrisonniers", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampPrisonniers destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserCampPrisonniers::~Mission_Automat_GEN_RealiserCampPrisonniers()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampPrisonniers::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserCampPrisonniers::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_prisonniers = &asnMission;

    const Location& camp_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( camp_, asnMission.camp );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.tc2 );

    delete &camp_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserCampPrisonniers::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserCampPrisonniers::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers );
    ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_camp_prisonniers;

    ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




