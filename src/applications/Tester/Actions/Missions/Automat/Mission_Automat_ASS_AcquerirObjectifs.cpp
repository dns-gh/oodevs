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
#include "Mission_Automat_ASS_AcquerirObjectifs.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_AcquerirObjectifs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_AcquerirObjectifs::Mission_Automat_ASS_AcquerirObjectifs( Automat& Automat )
    : Mission_Automat_ABC ( Automat )
{
    strName_ = "Automat_ASS_AcquerirObjectifs";
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_AcquerirObjectifs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_ASS_AcquerirObjectifs::~Mission_Automat_ASS_AcquerirObjectifs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_AcquerirObjectifs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_AcquerirObjectifs::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Automate_ASS_AcquerirObjectifs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_ass_acquerir_objectifs;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_ass_acquerir_objectifs = &asnMission;

    ASN_Tools::CopyPolygonList( pTarget_->GetTestParam_PolygonList(), asnMission.zones_a_observer );
    ASN_Tools::CopyPointList( pTarget_->GetTestParam_PointList(), asnMission.positions_deploiement );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.categories );

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_ass_acquerir_objectifs );
    ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_ass_acquerir_objectifs;


    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




