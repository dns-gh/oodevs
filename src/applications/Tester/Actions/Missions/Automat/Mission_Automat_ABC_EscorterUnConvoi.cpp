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
#include "Mission_Automat_ABC_EscorterUnConvoi.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnConvoi constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnConvoi::Mission_Automat_ABC_EscorterUnConvoi( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ABC_EscorterUnConvoi";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnConvoi destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ABC_EscorterUnConvoi::~Mission_Automat_ABC_EscorterUnConvoi()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnConvoi::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnConvoi::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ABC_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Automate_ABC_EscorterUnConvoi();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_abc_escorter_un_convoi;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_escorter_un_convoi = &asnMission;

    ASN_Tools::CopyAgent( pTarget_->GetTestParam_Agent(), asnMission.convoi_a_escorter );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ABC_EscorterUnConvoi::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ABC_EscorterUnConvoi::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_abc_escorter_un_convoi );
    ASN1T_Mission_Automate_ABC_EscorterUnConvoi& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_abc_escorter_un_convoi;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




