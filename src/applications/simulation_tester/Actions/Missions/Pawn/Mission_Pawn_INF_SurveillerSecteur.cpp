// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_INF_SurveillerSecteur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_SurveillerSecteur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_SurveillerSecteur::Mission_Pawn_INF_SurveillerSecteur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_SurveillerSecteur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_SurveillerSecteur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_SurveillerSecteur::~Mission_Pawn_INF_SurveillerSecteur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_SurveillerSecteur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_SurveillerSecteur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    Mission_Pion_INF_SurveillerSecteur& asnMission = *new Mission_Pion_INF_SurveillerSecteur();

    asnMsg_.GetAsnMsg().mission().mission_pion_inf_surveiller_secteur = &asnMission;

    const Position& positionInstallation_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionInstallation_, asnMission.position_installation );

    delete &positionInstallation_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_SurveillerSecteur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_SurveillerSecteur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_inf_surveiller_secteur ());
    Mission_Pion_INF_SurveillerSecteur& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_inf_surveiller_secteur;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}

