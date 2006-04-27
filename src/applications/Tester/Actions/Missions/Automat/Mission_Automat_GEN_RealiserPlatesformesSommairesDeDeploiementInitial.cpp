// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::~Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial = &asnMission;

    const T_GenObjectVector& platesFormes_ = pTarget_->GetTestParam_GenObjectList();

    ASN_Tools::CopyGenObjectList( platesFormes_, asnMission.plates_formes );

    delete &platesFormes_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial );
    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;

    ASN_Tools::Delete( asnMission.plates_formes );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




