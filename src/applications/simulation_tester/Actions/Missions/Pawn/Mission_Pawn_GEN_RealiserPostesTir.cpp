// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Pawn_GEN_RealiserPostesTir.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPostesTir constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserPostesTir::Mission_Pawn_GEN_RealiserPostesTir( Pawn& pawn )
    : Mission_Pawn_ABC ( "Pawn_GEN_RealiserPostesTir", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPostesTir destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_GEN_RealiserPostesTir::~Mission_Pawn_GEN_RealiserPostesTir()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPostesTir::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserPostesTir::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_GEN_RealiserPostesTir& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserPostesTir();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_gen_realiser_postes_tir;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_postes_tir = &asnMission;

    const Position& posObjet_ = pTarget_->GetTestParam_Point();

    ASN_Tools::CopyPoint( posObjet_, asnMission.pos_objet );

    delete &posObjet_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_GEN_RealiserPostesTir::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_GEN_RealiserPostesTir::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_gen_realiser_postes_tir );
    ASN1T_Mission_Pion_GEN_RealiserPostesTir& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_gen_realiser_postes_tir;

    ASN_Tools::Delete( asnMission.pos_objet );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


