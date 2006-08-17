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
#include "MIL_PionMission_RenseignerSurPopulations.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_RenseignerSurPopulations::nDIAPointIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RenseignerSurPopulations::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointIdx_ = DEC_Tools::InitializeDIAField( "point_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RenseignerSurPopulations::MIL_PionMission_RenseignerSurPopulations( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RenseignerSurPopulations::~MIL_PionMission_RenseignerSurPopulations()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_RenseignerSurPopulations::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *asnMsg.mission.u.mission_pion_renseigner_sur_populations;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point, GetVariable( nDIAPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_RenseignerSurPopulations::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_RenseignerSurPopulations::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RenseignerSurPopulations& mission = static_cast< MIL_PionMission_RenseignerSurPopulations& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointIdx_ ), GetVariable( nDIAPointIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RenseignerSurPopulations::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RenseignerSurPopulations::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *new ASN1T_Mission_Pion_RenseignerSurPopulations();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_renseigner_sur_populations;
    asnMsg.mission.u.mission_pion_renseigner_sur_populations  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointIdx_ ), asnMission.point );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RenseignerSurPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RenseignerSurPopulations::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_renseigner_sur_populations );
    ASN1T_Mission_Pion_RenseignerSurPopulations& asnMission = *asnMsg.mission.u.mission_pion_renseigner_sur_populations;

    NET_ASN_Tools::Delete( asnMission.point );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
