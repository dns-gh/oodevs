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
#include "Mission_Pawn_ASA_MISTRAL_Surveiller.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_Surveiller::Mission_Pawn_ASA_MISTRAL_Surveiller( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ASA_MISTRAL_Surveiller";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASA_MISTRAL_Surveiller::~Mission_Pawn_ASA_MISTRAL_Surveiller()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_Surveiller::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *new ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_asa_mistral_surveiller;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_surveiller = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_de_deploiement );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 1 ), asnMission.mode_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASA_MISTRAL_Surveiller::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASA_MISTRAL_Surveiller::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_asa_mistral_surveiller );
    ASN1T_Mission_Pion_ASA_MISTRAL_Surveiller& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_asa_mistral_surveiller;

    ASN_Tools::Delete( asnMission.point_de_deploiement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


