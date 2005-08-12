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
#include "Mission_Pawn_TRANS_EtablirGererLiaison.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_TRANS_EtablirGererLiaison constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_TRANS_EtablirGererLiaison::Mission_Pawn_TRANS_EtablirGererLiaison( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_TRANS_EtablirGererLiaison";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_TRANS_EtablirGererLiaison destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_TRANS_EtablirGererLiaison::~Mission_Pawn_TRANS_EtablirGererLiaison()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_TRANS_EtablirGererLiaison::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_TRANS_EtablirGererLiaison::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *new ASN1T_Mission_Pion_TRANS_EtablirGererLiaison();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_trans_etablir_gerer_liaison = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_TRANS_EtablirGererLiaison::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_TRANS_EtablirGererLiaison::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_trans_etablir_gerer_liaison );
    ASN1T_Mission_Pion_TRANS_EtablirGererLiaison& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_trans_etablir_gerer_liaison;

    ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


