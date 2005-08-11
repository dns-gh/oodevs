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
#include "Mission_Pawn_SeFaireTransporter.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireTransporter constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SeFaireTransporter::Mission_Pawn_SeFaireTransporter( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_SeFaireTransporter";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireTransporter destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_SeFaireTransporter::~Mission_Pawn_SeFaireTransporter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireTransporter::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_SeFaireTransporter::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *new ASN1T_Mission_Pion_SeFaireTransporter();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_se_faire_transporter;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_se_faire_transporter = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_embarquement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_SeFaireTransporter::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_SeFaireTransporter::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_se_faire_transporter );
    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_se_faire_transporter;

    ASN_Tools::Delete( asnMission.point_embarquement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


