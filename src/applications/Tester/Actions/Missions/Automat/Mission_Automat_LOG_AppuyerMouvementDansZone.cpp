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
#include "Mission_Automat_LOG_AppuyerMouvementDansZone.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvementDansZone::Mission_Automat_LOG_AppuyerMouvementDansZone( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_LOG_AppuyerMouvementDansZone";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_LOG_AppuyerMouvementDansZone::~Mission_Automat_LOG_AppuyerMouvementDansZone()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvementDansZone::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& asnMission = *new ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement_dans_zone = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_LOG_AppuyerMouvementDansZone::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_LOG_AppuyerMouvementDansZone::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_log_appuyer_mouvement_dans_zone );
    ASN1T_Mission_Automate_LOG_AppuyerMouvementDansZone& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_log_appuyer_mouvement_dans_zone;

    ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




