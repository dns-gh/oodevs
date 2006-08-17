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
#include "MIL_PionMission_NBC_AnimerUnPlotDeDecontamination.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::nDIAPositionSiteDecontaminationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionSiteDecontaminationIdx_ = DEC_Tools::InitializeDIAField( "positionSiteDecontamination_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::MIL_PionMission_NBC_AnimerUnPlotDeDecontamination( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::~MIL_PionMission_NBC_AnimerUnPlotDeDecontamination()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *asnMsg.mission.u.mission_pion_nbc_animer_un_plot_de_decontamination;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_site_decontamination, GetVariable( nDIAPositionSiteDecontaminationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_NBC_AnimerUnPlotDeDecontamination& mission = static_cast< MIL_PionMission_NBC_AnimerUnPlotDeDecontamination& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionSiteDecontaminationIdx_ ), GetVariable( nDIAPositionSiteDecontaminationIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *new ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination;
    asnMsg.mission.u.mission_pion_nbc_animer_un_plot_de_decontamination  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionSiteDecontaminationIdx_ ), asnMission.position_site_decontamination );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_NBC_AnimerUnPlotDeDecontamination::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_nbc_animer_un_plot_de_decontamination );
    ASN1T_Mission_Pion_NBC_AnimerUnPlotDeDecontamination& asnMission = *asnMsg.mission.u.mission_pion_nbc_animer_un_plot_de_decontamination;

    NET_ASN_Tools::Delete( asnMission.position_site_decontamination );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
