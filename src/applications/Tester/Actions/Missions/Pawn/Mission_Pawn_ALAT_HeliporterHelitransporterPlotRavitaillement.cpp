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
#include "Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Mission_Pawn_ABC ( pawn, nExecutionTick )
{
    strName_ = "Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement";
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::~Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Serialize()
{
    // build din/asn msg
    Mission_Pawn_ABC::Serialize();

    ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *new ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement();
    asnMsg_.GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;
    asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_heliporter_helitransporter_plot_ravitaillement = &asnMission;

    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.point_debarquement );
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), asnMission.position_regroupement );
    ASN_Tools::CopyObjectKnowledgeList( pTarget_->GetTestParam_ObjectKnowledgeList(), asnMission.plots_ravitaillement );
    ASN_Tools::CopyBool( pTarget_->GetTestParam_Bool(), asnMission.ravitaillement_debut_mission );
    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, 3 ), asnMission.portee_action );

}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ALAT_HeliporterHelitransporterPlotRavitaillement::Clean()
{
    assert( asnMsg_.GetAsnMsg().mission.t == T_Mission_Pion_mission_pion_alat_heliporter_helitransporter_plot_ravitaillement );
    ASN1T_Mission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement& asnMission = *asnMsg_.GetAsnMsg().mission.u.mission_pion_alat_heliporter_helitransporter_plot_ravitaillement;

    ASN_Tools::Delete( asnMission.point_debarquement );
    ASN_Tools::Delete( asnMission.position_regroupement );
    ASN_Tools::Delete( asnMission.plots_ravitaillement );


    delete &asnMission;
    Mission_Pawn_ABC::Clean();
}


