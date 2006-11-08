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
#include "MIL_PionMission_GEN_ReprendreTravaux.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_ReprendreTravaux::nDIATravauxIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_ReprendreTravaux::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATravauxIdx_ = DEC_Tools::InitializeDIAField( "travaux_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ReprendreTravaux::MIL_PionMission_GEN_ReprendreTravaux( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_ReprendreTravaux::~MIL_PionMission_GEN_ReprendreTravaux()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_ReprendreTravaux::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_ReprendreTravaux& asnMission = *asnMsg.mission.u.mission_pion_gen_reprendre_travaux;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.travaux, GetVariable( nDIATravauxIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ReprendreTravaux::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_ReprendreTravaux::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_ReprendreTravaux& mission = static_cast< MIL_PionMission_GEN_ReprendreTravaux& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIATravauxIdx_ ), GetVariable( nDIATravauxIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReprendreTravaux::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReprendreTravaux::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_ReprendreTravaux& asnMission = *new ASN1T_Mission_Pion_GEN_ReprendreTravaux();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_reprendre_travaux;
    asnMsg.mission.u.mission_pion_gen_reprendre_travaux  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIATravauxIdx_ ), asnMission.travaux, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_ReprendreTravaux::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_ReprendreTravaux::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_reprendre_travaux );
    ASN1T_Mission_Pion_GEN_ReprendreTravaux& asnMission = *asnMsg.mission.u.mission_pion_gen_reprendre_travaux;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
