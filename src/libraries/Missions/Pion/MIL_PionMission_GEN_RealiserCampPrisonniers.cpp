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
#include "MIL_PionMission_GEN_RealiserCampPrisonniers.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserCampPrisonniers::nDIACampIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserCampPrisonniers::nDIATc2Idx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserCampPrisonniers::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACampIdx_ = DEC_Tools::InitializeDIAField( "camp_", diaType );
    nDIATc2Idx_ = DEC_Tools::InitializeDIAField( "tc2_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserCampPrisonniers::MIL_PionMission_GEN_RealiserCampPrisonniers( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserCampPrisonniers::~MIL_PionMission_GEN_RealiserCampPrisonniers()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_camp_prisonniers;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.camp, camp_, GetVariable( nDIACampIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.tc2, GetVariable( nDIATc2Idx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( camp_, GetVariable( nDIACampIdx_ ) );
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIATc2Idx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserCampPrisonniers::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserCampPrisonniers& mission = static_cast< MIL_PionMission_GEN_RealiserCampPrisonniers& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIACampIdx_ ), camp_, GetVariable( nDIACampIdx_ ) );
    NET_ASN_Tools::CopyAutomate( mission.GetVariable( nDIATc2Idx_ ), GetVariable( nDIATc2Idx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampPrisonniers::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIATc2Idx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampPrisonniers::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers;
    asnMsg.mission.u.mission_pion_gen_realiser_camp_prisonniers  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIACampIdx_ ), asnMission.camp );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIATc2Idx_ ), asnMission.tc2 );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserCampPrisonniers::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserCampPrisonniers::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_camp_prisonniers );
    ASN1T_Mission_Pion_GEN_RealiserCampPrisonniers& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_camp_prisonniers;

    NET_ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
