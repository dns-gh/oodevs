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
#include "MIL_PionMission_ABC_ArmerPointDeControle.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_ArmerPointDeControle::nDIAPointDeControleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_ArmerPointDeControle::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointDeControleIdx_ = DEC_Tools::InitializeDIAField( "pointDeControle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ArmerPointDeControle::MIL_PionMission_ABC_ArmerPointDeControle( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ArmerPointDeControle::~MIL_PionMission_ABC_ArmerPointDeControle()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_ArmerPointDeControle::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *asnMsg.mission.u.mission_pion_abc_armer_point_de_controle;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_de_controle, GetVariable( nDIAPointDeControleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ArmerPointDeControle::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ArmerPointDeControle::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_ArmerPointDeControle& mission = static_cast< MIL_PionMission_ABC_ArmerPointDeControle& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointDeControleIdx_ ), GetVariable( nDIAPointDeControleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPointDeControle::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPointDeControle::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *new ASN1T_Mission_Pion_ABC_ArmerPointDeControle();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_armer_point_de_controle;
    asnMsg.mission.u.mission_pion_abc_armer_point_de_controle  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointDeControleIdx_ ), asnMission.point_de_controle );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ArmerPointDeControle::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ArmerPointDeControle::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_armer_point_de_controle );
    ASN1T_Mission_Pion_ABC_ArmerPointDeControle& asnMission = *asnMsg.mission.u.mission_pion_abc_armer_point_de_controle;

    NET_ASN_Tools::Delete( asnMission.point_de_controle );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
