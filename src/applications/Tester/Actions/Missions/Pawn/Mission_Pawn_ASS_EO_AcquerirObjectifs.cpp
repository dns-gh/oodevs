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
#include "Mission_Pawn_ASS_EO_AcquerirObjectifs.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EO_AcquerirObjectifs constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_EO_AcquerirObjectifs::Mission_Pawn_ASS_EO_AcquerirObjectifs( Pawn& pawn )
    : Mission_Pawn_ABC ( pawn )
{
    strName_ = "Pawn_ASS_EO_AcquerirObjectifs";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EO_AcquerirObjectifs destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ASS_EO_AcquerirObjectifs::~Mission_Pawn_ASS_EO_AcquerirObjectifs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EO_AcquerirObjectifs::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_EO_AcquerirObjectifs::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_eo_acquerir_objectifs = &asnMission;

    ASN_Tools::CopyPolygon( pTarget_->GetTestParam_Polygon(), asnMission.zone_a_observer );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.unite_a_appuyer );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.categories );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EO_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_EO_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs );
    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_ass_eo_acquerir_objectifs;

    ASN_Tools::Delete( asnMission.zone_a_observer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


