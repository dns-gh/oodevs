// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_RENS_ROIM_CL289_RenseignerSur.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROIM_CL289_RenseignerSur constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROIM_CL289_RenseignerSur::Mission_Automat_RENS_ROIM_CL289_RenseignerSur( Automat& Automat )
    : Mission_Automat_ABC ( "Automat_RENS_ROIM_CL289_RenseignerSur", Automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROIM_CL289_RenseignerSur destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Automat_RENS_ROIM_CL289_RenseignerSur::~Mission_Automat_RENS_ROIM_CL289_RenseignerSur()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROIM_CL289_RenseignerSur::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROIM_CL289_RenseignerSur::Serialize()
{
    // build din/asn msg
    Mission_Automat_ABC::Serialize();

    ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur;
    asnMsg_.GetAsnMsg().mission.u.mission_automate_rens_roim_cl289_renseigner_sur = &asnMission;

    const Path& ficelleDeVole_ = pTarget_->GetTestParam_Path();
    const T_LocationVector& zonesDeRecherche_ = pTarget_->GetTestParam_PolygonList();

    ASN_Tools::CopyPath( ficelleDeVole_, asnMission.ficelle_de_vole );
    ASN_Tools::CopyPolygonList( zonesDeRecherche_, asnMission.zones_de_recherche );

    delete &ficelleDeVole_;
    delete &zonesDeRecherche_;

}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_RENS_ROIM_CL289_RenseignerSur::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Automat_RENS_ROIM_CL289_RenseignerSur::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Automate_mission_automate_rens_roim_cl289_renseigner_sur );
    ASN1T_Mission_Automate_RENS_ROIM_CL289_RenseignerSur& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_automate_rens_roim_cl289_renseigner_sur;

    ASN_Tools::Delete( asnMission.ficelle_de_vole );
    ASN_Tools::Delete( asnMission.zones_de_recherche );

    delete &asnMission;
    Mission_Automat_ABC::Clean();
}




