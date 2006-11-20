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
#include "MIL_PionMission_GEN_RealiserTravauxProtection.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserTravauxProtection::nDIATravauxIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserTravauxProtection::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATravauxIdx_ = DEC_Tools::InitializeDIAField( "travaux_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxProtection::MIL_PionMission_GEN_RealiserTravauxProtection( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxProtection::~MIL_PionMission_GEN_RealiserTravauxProtection()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.travaux, GetVariable( nDIATravauxIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserTravauxProtection& mission = static_cast< MIL_PionMission_GEN_RealiserTravauxProtection& >( missionTmp );

    NET_ASN_Tools::CopyGenObjectList( mission.GetVariable( nDIATravauxIdx_ ), GetVariable( nDIATravauxIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxProtection::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIATravauxIdx_ ), asnMission.travaux );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxProtection::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_protection );
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection;

    NET_ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
