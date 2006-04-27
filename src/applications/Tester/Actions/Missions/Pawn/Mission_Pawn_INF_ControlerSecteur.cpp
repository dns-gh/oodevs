// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_INF_ControlerSecteur.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ControlerSecteur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_ControlerSecteur::Mission_Pawn_INF_ControlerSecteur( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_INF_ControlerSecteur", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ControlerSecteur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_INF_ControlerSecteur::~Mission_Pawn_INF_ControlerSecteur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ControlerSecteur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_ControlerSecteur::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_ControlerSecteur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_controler_secteur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_controler_secteur = &asnMission;

    const Location& secteur_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( secteur_, asnMission.secteur );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.preparer_terrain );

    delete &secteur_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_ControlerSecteur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_ControlerSecteur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_controler_secteur );
    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_controler_secteur;

    ASN_Tools::Delete( asnMission.secteur );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


