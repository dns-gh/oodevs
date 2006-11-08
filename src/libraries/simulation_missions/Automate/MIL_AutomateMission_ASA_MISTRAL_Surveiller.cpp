// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_ASA_MISTRAL_Surveiller.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASA_MISTRAL_Surveiller::nDIAZoneIdx_ = 0 ;
int MIL_AutomateMission_ASA_MISTRAL_Surveiller::nDIAModeDeploiementIdx_ = 0 ;
int MIL_AutomateMission_ASA_MISTRAL_Surveiller::nDIAAngleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_MISTRAL_Surveiller::MIL_AutomateMission_ASA_MISTRAL_Surveiller( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_MISTRAL_Surveiller::~MIL_AutomateMission_ASA_MISTRAL_Surveiller()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASA_MISTRAL_Surveiller::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAModeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "modeDeploiement_", diaType );
    nDIAAngleIdx_ = DEC_Tools::InitializeDIAField( "angle_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASA_MISTRAL_Surveiller::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& asnMission = *asnMsg.mission.u.mission_automate_asa_mistral_surveiller;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.mode_deploiement, GetVariable( nDIAModeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.angle, GetVariable( nDIAAngleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_Surveiller::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_Surveiller::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_asa_mistral_surveiller;
    asnMsg.mission.u.mission_automate_asa_mistral_surveiller  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAModeDeploiementIdx_ ), asnMission.mode_deploiement );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIAAngleIdx_ ), asnMission.angle );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_Surveiller::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_Surveiller::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_asa_mistral_surveiller );
    ASN1T_Mission_Automate_ASA_MISTRAL_Surveiller& asnMission = *asnMsg.mission.u.mission_automate_asa_mistral_surveiller;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
