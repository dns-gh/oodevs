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
#include "MIL_PionMission_InterdireFranchissementPopulations.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_InterdireFranchissementPopulations::nDIAPointIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_InterdireFranchissementPopulations::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointIdx_ = DEC_Tools::InitializeDIAField( "point_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_InterdireFranchissementPopulations::MIL_PionMission_InterdireFranchissementPopulations( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_InterdireFranchissementPopulations::~MIL_PionMission_InterdireFranchissementPopulations()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_InterdireFranchissementPopulations::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *asnMsg.mission.u.mission_pion_interdire_franchissement_populations;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point, GetVariable( nDIAPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_InterdireFranchissementPopulations::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_InterdireFranchissementPopulations::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_InterdireFranchissementPopulations& mission = static_cast< MIL_PionMission_InterdireFranchissementPopulations& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointIdx_ ), GetVariable( nDIAPointIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulations::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulations::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *new ASN1T_Mission_Pion_InterdireFranchissementPopulations();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_interdire_franchissement_populations;
    asnMsg.mission.u.mission_pion_interdire_franchissement_populations  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointIdx_ ), asnMission.point );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulations::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_interdire_franchissement_populations );
    ASN1T_Mission_Pion_InterdireFranchissementPopulations& asnMission = *asnMsg.mission.u.mission_pion_interdire_franchissement_populations;

    NET_ASN_Tools::Delete( asnMission.point );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
