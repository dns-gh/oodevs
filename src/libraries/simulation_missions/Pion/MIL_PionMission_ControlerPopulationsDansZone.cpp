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
#include "MIL_PionMission_ControlerPopulationsDansZone.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ControlerPopulationsDansZone::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ControlerPopulationsDansZone::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ControlerPopulationsDansZone::MIL_PionMission_ControlerPopulationsDansZone( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ControlerPopulationsDansZone::~MIL_PionMission_ControlerPopulationsDansZone()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ControlerPopulationsDansZone::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ControlerPopulationsDansZone& asnMission = *asnMsg.mission.u.mission_pion_controler_populations_dans_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ControlerPopulationsDansZone::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ControlerPopulationsDansZone::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ControlerPopulationsDansZone& mission = static_cast< MIL_PionMission_ControlerPopulationsDansZone& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ControlerPopulationsDansZone::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ControlerPopulationsDansZone& asnMission = *new ASN1T_Mission_Pion_ControlerPopulationsDansZone();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_controler_populations_dans_zone;
    asnMsg.mission.u.mission_pion_controler_populations_dans_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ControlerPopulationsDansZone::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ControlerPopulationsDansZone::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_controler_populations_dans_zone );
    ASN1T_Mission_Pion_ControlerPopulationsDansZone& asnMission = *asnMsg.mission.u.mission_pion_controler_populations_dans_zone;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
