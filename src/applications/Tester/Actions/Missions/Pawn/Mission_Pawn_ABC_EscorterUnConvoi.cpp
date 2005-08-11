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
#include "Mission_Pawn_ABC_EscorterUnConvoi.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnConvoi constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_EscorterUnConvoi::Mission_Pawn_ABC_EscorterUnConvoi( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ABC_EscorterUnConvoi";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnConvoi destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ABC_EscorterUnConvoi::~Mission_Pawn_ABC_EscorterUnConvoi()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnConvoi::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_EscorterUnConvoi::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ABC_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Pion_ABC_EscorterUnConvoi();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_abc_escorter_un_convoi;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_escorter_un_convoi = &asnMission;

    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.convoi_a_escorter );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC_EscorterUnConvoi::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC_EscorterUnConvoi::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_abc_escorter_un_convoi );
    ASN1T_Mission_Pion_ABC_EscorterUnConvoi& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_abc_escorter_un_convoi;



    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


