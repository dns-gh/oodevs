// *****************************************************************************
//
// $Created: 2005-4-27 - 17:37:45 $
// $Archive: /MVW_v10/Build/SDK/Missions/src/Automate/MIL_AutomateMission_ALAT_Controler.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:37 $
// $Revision: 33 $
// $Workfile: MIL_AutomateMission_ALAT_Controler.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ALAT_Controler.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

int MIL_AutomateMission_ALAT_Controler::nDIAZoneIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Controler::nDIAPointRegroupementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Controler::nDIAPlotsRavitaillementIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Controler::nDIAPorteeActionIdx_ = 0 ;
int MIL_AutomateMission_ALAT_Controler::nDIAAmbianceMissionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler constructor
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_Controler::MIL_AutomateMission_ALAT_Controler( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler destructor
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
MIL_AutomateMission_ALAT_Controler::~MIL_AutomateMission_ALAT_Controler()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ALAT_Controler::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAPlotsRavitaillementIdx_ = DEC_Tools::InitializeDIAField( "plotsRavitaillement_", diaType );
    nDIAPorteeActionIdx_ = DEC_Tools::InitializeDIAField( "porteeAction_", diaType );
    nDIAAmbianceMissionIdx_ = DEC_Tools::InitializeDIAField( "ambianceMission_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler::Initialize
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ALAT_Controler::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ALAT_Controler& asnMission = *asnMsg.mission.u.mission_automate_alat_controler;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
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
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Controler::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler::Serialize
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Controler::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ALAT_Controler& asnMission = *new ASN1T_Mission_Automate_ALAT_Controler();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_alat_controler;
    asnMsg.mission.u.mission_automate_alat_controler  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAPlotsRavitaillementIdx_ ), asnMission.plots_ravitaillement, automate_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAPorteeActionIdx_ ), asnMission.portee_action );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAAmbianceMissionIdx_ ), asnMission.ambiance_mission );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ALAT_Controler::CleanAfterSerialization
// Created: 2005-4-27 - 17:37:45
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ALAT_Controler::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_alat_controler );
    ASN1T_Mission_Automate_ALAT_Controler& asnMission = *asnMsg.mission.u.mission_automate_alat_controler;

    NET_ASN_Tools::Delete( asnMission.zone );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );
    NET_ASN_Tools::Delete( asnMission.plots_ravitaillement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
