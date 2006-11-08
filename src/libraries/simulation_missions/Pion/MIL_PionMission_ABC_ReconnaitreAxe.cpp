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
#include "MIL_PionMission_ABC_ReconnaitreAxe.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_ReconnaitreAxe::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_ReconnaitreAxe::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ReconnaitreAxe::MIL_PionMission_ABC_ReconnaitreAxe( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_ReconnaitreAxe::~MIL_PionMission_ABC_ReconnaitreAxe()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_ReconnaitreAxe::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *asnMsg.mission.u.mission_pion_abc_reconnaitre_axe;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ReconnaitreAxe::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_ReconnaitreAxe::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_ReconnaitreAxe& mission = static_cast< MIL_PionMission_ABC_ReconnaitreAxe& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitreAxe::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitreAxe::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *new ASN1T_Mission_Pion_ABC_ReconnaitreAxe();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_reconnaitre_axe;
    asnMsg.mission.u.mission_pion_abc_reconnaitre_axe  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_ReconnaitreAxe::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_ReconnaitreAxe::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_reconnaitre_axe );
    ASN1T_Mission_Pion_ABC_ReconnaitreAxe& asnMission = *asnMsg.mission.u.mission_pion_abc_reconnaitre_axe;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
