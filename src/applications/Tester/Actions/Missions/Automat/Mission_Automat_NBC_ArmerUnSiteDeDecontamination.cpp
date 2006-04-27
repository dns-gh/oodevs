// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_NBC_ArmerUnSiteDeDecontamination.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ArmerUnSiteDeDecontamination constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ArmerUnSiteDeDecontamination::Mission_Automat_NBC_ArmerUnSiteDeDecontamination( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_NBC_ArmerUnSiteDeDecontamination", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ArmerUnSiteDeDecontamination destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_NBC_ArmerUnSiteDeDecontamination::~Mission_Automat_NBC_ArmerUnSiteDeDecontamination()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ArmerUnSiteDeDecontamination::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ArmerUnSiteDeDecontamination::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *new ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_armer_un_site_de_decontamination = &asnMission;

    const Location& site_ = pTarget_->GetTestParam_Polygon();

    ASN_Tools::CopyPolygon( site_, asnMission.site );

    delete &site_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_NBC_ArmerUnSiteDeDecontamination::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_NBC_ArmerUnSiteDeDecontamination::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_nbc_armer_un_site_de_decontamination );
    ASN1T_Mission_Automate_NBC_ArmerUnSiteDeDecontamination& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_nbc_armer_un_site_de_decontamination;

    ASN_Tools::Delete( asnMission.site );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




