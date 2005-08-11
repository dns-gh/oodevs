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
#include "Mission_Pawn_RENS_ROEM_MettreEnOeuvre.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_MettreEnOeuvre constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_MettreEnOeuvre::Mission_Pawn_RENS_ROEM_MettreEnOeuvre( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_RENS_ROEM_MettreEnOeuvre";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_MettreEnOeuvre destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_RENS_ROEM_MettreEnOeuvre::~Mission_Pawn_RENS_ROEM_MettreEnOeuvre()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_MettreEnOeuvre::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_MettreEnOeuvre::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_mettre_en_oeuvre = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_deploiement );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_RENS_ROEM_MettreEnOeuvre::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_RENS_ROEM_MettreEnOeuvre::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_rens_roem_mettre_en_oeuvre );
    ASN1T_Mission_Pion_RENS_ROEM_MettreEnOeuvre& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_rens_roem_mettre_en_oeuvre;

    ASN_Tools::Delete( asnMission.position_deploiement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


