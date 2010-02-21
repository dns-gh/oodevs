// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
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
    : Mission_Automat_ABC ( "Automat_ASS_AcquerirObjectifs", Automat )
{
    // NOTHING
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

    Mission_Automate_ASS_AcquerirObjectifs& asnMission = *new Mission_Automate_ASS_AcquerirObjectifs();

    asnMsg_.GetAsnMsg().mission().mission_automate_ass_acquerir_objectifs = &asnMission;

    const T_LocationVector& zonesAObserver_ = pTarget_->GetTestParam_PolygonList();
    const T_PositionVector& positionsDeploiement_ = pTarget_->GetTestParam_PointList();

    ASN_Tools::CopyPolygonList( zonesAObserver_, asnMission.zones_a_observer );
    ASN_Tools::CopyPointList( positionsDeploiement_, asnMission.positions_deploiement );
    ASN_Tools::CopyNatureAtlas( pTarget_->GetTestParam_NatureAtlas(), asnMission.categories );

    delete &zonesAObserver_;
    delete &positionsDeploiement_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_ASS_AcquerirObjectifs::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_ASS_AcquerirObjectifs::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.has_mission_automate_ass_acquerir_objectifs ());
    Mission_Automate_ASS_AcquerirObjectifs& asnMission = *asnMsg_.GetAsnMsg().mission().mission_automate_ass_acquerir_objectifs;

    ASN_Tools::Delete( asnMission.zones_a_observer );
    ASN_Tools::Delete( asnMission.positions_deploiement );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




