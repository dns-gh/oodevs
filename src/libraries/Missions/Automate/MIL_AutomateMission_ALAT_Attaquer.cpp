// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ALAT_Attaquer.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ALAT_Attaquer::nDIACiblesPrioritairesIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Attaquer::nDIAPointRegroupementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Attaquer::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Attaquer::nDIAPorteeActionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Attaquer::nDIAAmbianceMissionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_Attaquer::MIL_AutomateMission_ALAT_Attaquer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_Attaquer::~MIL_AutomateMission_ALAT_Attaquer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ALAT_Attaquer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACiblesPrioritairesIdx_ = DEC_Tools::InitializeDIAField( "ciblesPrioritaires_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ALAT_Attaquer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ALAT_Attaquer& asnMission = *asnMsg.mission.u.mission_automate_alat_attaquer;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.cibles_prioritaires, GetVariable( nDIACiblesPrioritairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.plots_ravitaillement, GetVariable( nDIAPlotsRavitaillementIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.portee_action, GetVariable( nDIAPorteeActionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.ambiance_mission, GetVariable( nDIAAmbianceMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Attaquer::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Attaquer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ALAT_Attaquer& asnMission = *new ASN1T_Mission_Automate_ALAT_Attaquer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_alat_attaquer;
    asnMsg.mission.u.mission_automate_alat_attaquer  = &asnMission;

    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACiblesPrioritairesIdx_ ), asnMission.cibles_prioritaires );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, automate_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Attaquer::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Attaquer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_alat_attaquer );
    ASN1T_Mission_Automate_ALAT_Attaquer& asnMission = *asnMsg.mission.u.mission_automate_alat_attaquer;

    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
