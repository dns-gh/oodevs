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
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial";
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

    ASN_Tools::CopyGenObjectList( pTarget_->GetTestParam_GenObjectList(), asnMission.plates_formes );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial );
    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




