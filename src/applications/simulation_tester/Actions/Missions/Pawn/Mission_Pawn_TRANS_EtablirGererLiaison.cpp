// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Pawn_ABC ( "Pawn_TRANS_EtablirGererLiaison", pawn )
{
    // NOTHING
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

    const Position& positionDeploiement_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( positionDeploiement_, asnMission.position_deploiement );

    delete &positionDeploiement_;

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


