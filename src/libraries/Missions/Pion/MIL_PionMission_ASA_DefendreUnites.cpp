// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ASA_DefendreUnites.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_DefendreUnites::nDIAUnitesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_DefendreUnites::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUnitesIdx_ = DEC_Tools::InitializeDIAField( "unites_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreUnites::MIL_PionMission_ASA_DefendreUnites( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreUnites::~MIL_PionMission_ASA_DefendreUnites()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_DefendreUnites::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_unites;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites, GetVariable( nDIAUnitesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreUnites::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreUnites::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_DefendreUnites& mission = static_cast< MIL_PionMission_ASA_DefendreUnites& >( missionTmp );

    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesIdx_ ), GetVariable( nDIAUnitesIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreUnites::Terminate()
{
    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUnitesIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreUnites::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreUnites();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_defendre_unites;
    asnMsg.mission.u.mission_pion_asa_defendre_unites  = &asnMission;

    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesIdx_ ), asnMission.unites );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreUnites::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreUnites::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_defendre_unites );
    ASN1T_Mission_Pion_ASA_DefendreUnites& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_unites;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
