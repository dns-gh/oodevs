// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_PionMission_JOINT_MARINE_Transporter.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_JOINT_MARINE_Transporter::nDIAUnitesATransporterIdx_ = 0 ;
int MIL_PionMission_JOINT_MARINE_Transporter::nDIAPointDebarquementIdx_ = 0 ;
int MIL_PionMission_JOINT_MARINE_Transporter::nDIAPointEmbarquementIdx_ = 0 ;
int MIL_PionMission_JOINT_MARINE_Transporter::nDIAAvecMaterielIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_JOINT_MARINE_Transporter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesATransporterIdx_ = DEC_Tools::InitializeDIAField( "unitesATransporter_", diaType );
    nDIAPointDebarquementIdx_ = DEC_Tools::InitializeDIAField( "pointDebarquement_", diaType );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );
    nDIAAvecMaterielIdx_ = DEC_Tools::InitializeDIAField( "avecMateriel_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_JOINT_MARINE_Transporter::MIL_PionMission_JOINT_MARINE_Transporter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_JOINT_MARINE_Transporter::~MIL_PionMission_JOINT_MARINE_Transporter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_JOINT_MARINE_Transporter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *asnMsg.mission.u.mission_pion_joint_marine_transporter;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_transporter, GetVariable( nDIAUnitesATransporterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_debarquement, GetVariable( nDIAPointDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.avec_materiel, GetVariable( nDIAAvecMaterielIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_JOINT_MARINE_Transporter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

                
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_JOINT_MARINE_Transporter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_JOINT_MARINE_Transporter& mission = static_cast< MIL_PionMission_JOINT_MARINE_Transporter& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesATransporterIdx_ ), GetVariable( nDIAUnitesATransporterIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDebarquementIdx_ ), GetVariable( nDIAPointDebarquementIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointEmbarquementIdx_ ), GetVariable( nDIAPointEmbarquementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAAvecMaterielIdx_ ), GetVariable( nDIAAvecMaterielIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_JOINT_MARINE_Transporter::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_JOINT_MARINE_Transporter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *new ASN1T_Mission_Pion_JOINT_MARINE_Transporter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_joint_marine_transporter;
    asnMsg.mission.u.mission_pion_joint_marine_transporter  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesATransporterIdx_ ), asnMission.unites_a_transporter );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDebarquementIdx_ ), asnMission.point_debarquement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAvecMaterielIdx_ ), asnMission.avec_materiel );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_JOINT_MARINE_Transporter::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_JOINT_MARINE_Transporter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_joint_marine_transporter );
    ASN1T_Mission_Pion_JOINT_MARINE_Transporter& asnMission = *asnMsg.mission.u.mission_pion_joint_marine_transporter;

    NET_ASN_Tools::Delete( asnMission.unites_a_transporter );
    NET_ASN_Tools::Delete( asnMission.point_debarquement );
    NET_ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
