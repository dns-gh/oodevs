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
#include "MIL_PionMission_ABC_Couvrir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_Couvrir::nDIAPositionIdx_ = 0 ;
int MIL_PionMission_ABC_Couvrir::nDIAUnitesACouvrirIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_Couvrir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionIdx_ = DEC_Tools::InitializeDIAField( "position_", diaType );
    nDIAUnitesACouvrirIdx_ = DEC_Tools::InitializeDIAField( "unitesACouvrir_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Couvrir::MIL_PionMission_ABC_Couvrir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Couvrir::~MIL_PionMission_ABC_Couvrir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_Couvrir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *asnMsg.mission.u.mission_pion_abc_couvrir;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position, position_, GetVariable( nDIAPositionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites_a_couvrir, GetVariable( nDIAUnitesACouvrirIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Couvrir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( position_, GetVariable( nDIAPositionIdx_ ) );
    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesACouvrirIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Couvrir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_Couvrir& mission = static_cast< MIL_PionMission_ABC_Couvrir& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionIdx_ ), position_, GetVariable( nDIAPositionIdx_ ) );
    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesACouvrirIdx_ ), GetVariable( nDIAUnitesACouvrirIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Couvrir::Terminate()
{
    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesACouvrirIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Couvrir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *new ASN1T_Mission_Pion_ABC_Couvrir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_couvrir;
    asnMsg.mission.u.mission_pion_abc_couvrir  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionIdx_ ), asnMission.position );
    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesACouvrirIdx_ ), asnMission.unites_a_couvrir );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Couvrir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Couvrir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_couvrir );
    ASN1T_Mission_Pion_ABC_Couvrir& asnMission = *asnMsg.mission.u.mission_pion_abc_couvrir;

    NET_ASN_Tools::Delete( asnMission.position );
    NET_ASN_Tools::Delete( asnMission.unites_a_couvrir );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
