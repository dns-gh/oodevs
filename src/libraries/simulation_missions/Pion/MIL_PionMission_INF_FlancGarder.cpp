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
#include "MIL_PionMission_INF_FlancGarder.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_FlancGarder::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_INF_FlancGarder::nDIAUniteAFlancgarderIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_FlancGarder::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAUniteAFlancgarderIdx_ = DEC_Tools::InitializeDIAField( "uniteAFlancgarder_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_FlancGarder::MIL_PionMission_INF_FlancGarder( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_FlancGarder::~MIL_PionMission_INF_FlancGarder()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_FlancGarder::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *asnMsg.mission.u.mission_pion_inf_flanc_garder;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_flancgarder, GetVariable( nDIAUniteAFlancgarderIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_FlancGarder::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_FlancGarder::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_FlancGarder& mission = static_cast< MIL_PionMission_INF_FlancGarder& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteAFlancgarderIdx_ ), GetVariable( nDIAUniteAFlancgarderIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_FlancGarder::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_FlancGarder::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *new ASN1T_Mission_Pion_INF_FlancGarder();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_flanc_garder;
    asnMsg.mission.u.mission_pion_inf_flanc_garder  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteAFlancgarderIdx_ ), asnMission.unite_a_flancgarder );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_FlancGarder::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_FlancGarder::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_flanc_garder );
    ASN1T_Mission_Pion_INF_FlancGarder& asnMission = *asnMsg.mission.u.mission_pion_inf_flanc_garder;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
