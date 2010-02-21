// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Pawn_ABC ( "Pawn_ASS_EO_AcquerirObjectifs", pawn )
{
    // NOTHING
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

    Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *new Mission_Pion_ASS_EO_AcquerirObjectifs();

    asnMsg_.GetAsnMsg().mission().mission_pion_ass_eo_acquerir_objectifs = &asnMission;

    const Location& zoneAObserver_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( zoneAObserver_, asnMission.zone_a_observer );
    ASN_Tools::CopyAutomate( pTarget_->GetTestParam_Automate(), asnMission.unite_a_appuyer );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.categories );

    delete &zoneAObserver_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ASS_EO_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ASS_EO_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_pion_ass_eo_acquerir_objectifs ());
    Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission().mission_pion_ass_eo_acquerir_objectifs;

    ASN_Tools::Delete( asnMission.zone_a_observer );

    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


