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
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_INF_SurveillerSecteur";
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

    ASN1T_Mission_Pion_INF_SurveillerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_SurveillerSecteur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_inf_surveiller_secteur;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_surveiller_secteur = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_installation );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_INF_SurveillerSecteur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_INF_SurveillerSecteur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_inf_surveiller_secteur );
    ASN1T_Mission_Pion_INF_SurveillerSecteur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_inf_surveiller_secteur;

    ASN_Tools::Delete( asnMission.position_installation );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


