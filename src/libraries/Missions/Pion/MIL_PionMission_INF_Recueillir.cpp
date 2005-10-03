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

#include "Missions_pch.h"
#include "MIL_PionMission_INF_Recueillir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_Recueillir::nDIAPositionInstallationIdx_ = 0 ;
int MIL_PionMission_INF_Recueillir::nDIAPiaIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_Recueillir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionInstallationIdx_ = DEC_Tools::InitializeDIAField( "positionInstallation_", diaType );
    nDIAPiaIdx_ = DEC_Tools::InitializeDIAField( "pia_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Recueillir::MIL_PionMission_INF_Recueillir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_Recueillir::~MIL_PionMission_INF_Recueillir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_Recueillir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_Recueillir& asnMission = *asnMsg.mission.u.mission_pion_inf_recueillir;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_installation, positionInstallation_, GetVariable( nDIAPositionInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pia, pia_, GetVariable( nDIAPiaIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_Recueillir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( positionInstallation_, GetVariable( nDIAPositionInstallationIdx_ ) );
    NET_ASN_Tools::ResetPoint( pia_, GetVariable( nDIAPiaIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_Recueillir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_Recueillir& mission = static_cast< MIL_PionMission_INF_Recueillir& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionInstallationIdx_ ), positionInstallation_, GetVariable( nDIAPositionInstallationIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPiaIdx_ ), pia_, GetVariable( nDIAPiaIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Recueillir::Terminate()
{
    NET_ASN_Tools::ResetPoint( pia_, GetVariable( nDIAPiaIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Recueillir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *new ASN1T_Mission_Pion_INF_Recueillir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_recueillir;
    asnMsg.mission.u.mission_pion_inf_recueillir  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionInstallationIdx_ ), asnMission.position_installation );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPiaIdx_ ), asnMission.pia );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_Recueillir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_Recueillir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_recueillir );
    ASN1T_Mission_Pion_INF_Recueillir& asnMission = *asnMsg.mission.u.mission_pion_inf_recueillir;

    NET_ASN_Tools::Delete( asnMission.position_installation );
    NET_ASN_Tools::Delete( asnMission.pia );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
