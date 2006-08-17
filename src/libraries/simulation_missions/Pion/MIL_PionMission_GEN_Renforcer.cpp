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
#include "MIL_PionMission_GEN_Renforcer.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_Renforcer::nDIALieuRenforcementIdx_ = 0 ;
int MIL_PionMission_GEN_Renforcer::nDIAUniteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_Renforcer::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIALieuRenforcementIdx_ = DEC_Tools::InitializeDIAField( "lieuRenforcement_", diaType );
    nDIAUniteIdx_ = DEC_Tools::InitializeDIAField( "unite_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_Renforcer::MIL_PionMission_GEN_Renforcer( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_Renforcer::~MIL_PionMission_GEN_Renforcer()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_Renforcer::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_Renforcer& asnMission = *asnMsg.mission.u.mission_pion_gen_renforcer;
    if( !NET_ASN_Tools::CopyPoint( asnMission.lieu_renforcement, GetVariable( nDIALieuRenforcementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite, GetVariable( nDIAUniteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_Renforcer::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_Renforcer::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_Renforcer& mission = static_cast< MIL_PionMission_GEN_Renforcer& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIALieuRenforcementIdx_ ), GetVariable( nDIALieuRenforcementIdx_ ) );
    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteIdx_ ), GetVariable( nDIAUniteIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_Renforcer::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_Renforcer::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_Renforcer& asnMission = *new ASN1T_Mission_Pion_GEN_Renforcer();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_renforcer;
    asnMsg.mission.u.mission_pion_gen_renforcer  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIALieuRenforcementIdx_ ), asnMission.lieu_renforcement );
    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteIdx_ ), asnMission.unite );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_Renforcer::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_Renforcer::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_renforcer );
    ASN1T_Mission_Pion_GEN_Renforcer& asnMission = *asnMsg.mission.u.mission_pion_gen_renforcer;

    NET_ASN_Tools::Delete( asnMission.lieu_renforcement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
