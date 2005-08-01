// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_GEN_RealiserCampRefugies.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserCampRefugies::nDIACampIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserCampRefugies::nDIATc2Idx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserCampRefugies::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACampIdx_ = DEC_Tools::InitializeDIAField( "camp_", diaType );
    nDIATc2Idx_ = DEC_Tools::InitializeDIAField( "tc2_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserCampRefugies::MIL_PionMission_GEN_RealiserCampRefugies( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserCampRefugies::~MIL_PionMission_GEN_RealiserCampRefugies()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserCampRefugies::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_camp_refugies;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.camp, camp_, GetVariable( nDIACampIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.tc2, GetVariable( nDIATc2Idx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserCampRefugies::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( camp_, GetVariable( nDIACampIdx_ ) );
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIATc2Idx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserCampRefugies::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserCampRefugies& mission = static_cast< MIL_PionMission_GEN_RealiserCampRefugies& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIACampIdx_ ), camp_, GetVariable( nDIACampIdx_ ) );
    NET_ASN_Tools::CopyAutomate( mission.GetVariable( nDIATc2Idx_ ), GetVariable( nDIATc2Idx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampRefugies::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIATc2Idx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampRefugies::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampRefugies();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_camp_refugies;
    asnMsg.mission.u.mission_pion_gen_realiser_camp_refugies  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIACampIdx_ ), asnMission.camp );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIATc2Idx_ ), asnMission.tc2 );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampRefugies::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampRefugies::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_camp_refugies );
    ASN1T_Mission_Pion_GEN_RealiserCampRefugies& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_camp_refugies;

    NET_ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
