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
#include "MIL_PionMission_PlastronEnnemi.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_PlastronEnnemi::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_PlastronEnnemi::nDIAPointMissionIdx_ = 0 ;
int MIL_PionMission_PlastronEnnemi::nDIATypeIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_PlastronEnnemi::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIAPointMissionIdx_ = DEC_Tools::InitializeDIAField( "pointMission_", diaType );
    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "type_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_PlastronEnnemi::MIL_PionMission_PlastronEnnemi( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_PlastronEnnemi::~MIL_PionMission_PlastronEnnemi()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_PlastronEnnemi::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *asnMsg.mission.u.mission_pion_plastron_ennemi;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_mission, GetVariable( nDIAPointMissionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type, GetVariable( nDIATypeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_PlastronEnnemi::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

            
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_PlastronEnnemi::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_PlastronEnnemi& mission = static_cast< MIL_PionMission_PlastronEnnemi& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointMissionIdx_ ), GetVariable( nDIAPointMissionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeIdx_ ), GetVariable( nDIATypeIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_PlastronEnnemi::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_PlastronEnnemi::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *new ASN1T_Mission_Pion_PlastronEnnemi();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_plastron_ennemi;
    asnMsg.mission.u.mission_pion_plastron_ennemi  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointMissionIdx_ ), asnMission.point_mission );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeIdx_ ), asnMission.type );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_PlastronEnnemi::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_PlastronEnnemi::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_plastron_ennemi );
    ASN1T_Mission_Pion_PlastronEnnemi& asnMission = *asnMsg.mission.u.mission_pion_plastron_ennemi;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.point_mission );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
