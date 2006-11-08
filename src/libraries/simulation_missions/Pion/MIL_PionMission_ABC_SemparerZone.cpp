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
#include "MIL_PionMission_ABC_SemparerZone.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_SemparerZone::nDIAZoneAPrendreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_SemparerZone::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneAPrendreIdx_ = DEC_Tools::InitializeDIAField( "zoneAPrendre_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_SemparerZone::MIL_PionMission_ABC_SemparerZone( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_SemparerZone::~MIL_PionMission_ABC_SemparerZone()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_SemparerZone::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *asnMsg.mission.u.mission_pion_abc_semparer_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_a_prendre, GetVariable( nDIAZoneAPrendreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_SemparerZone::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_SemparerZone::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_SemparerZone& mission = static_cast< MIL_PionMission_ABC_SemparerZone& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneAPrendreIdx_ ), GetVariable( nDIAZoneAPrendreIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_SemparerZone::Terminate()
{
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_SemparerZone::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *new ASN1T_Mission_Pion_ABC_SemparerZone();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_semparer_zone;
    asnMsg.mission.u.mission_pion_abc_semparer_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneAPrendreIdx_ ), asnMission.zone_a_prendre );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_SemparerZone::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_SemparerZone::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_semparer_zone );
    ASN1T_Mission_Pion_ABC_SemparerZone& asnMission = *asnMsg.mission.u.mission_pion_abc_semparer_zone;

    NET_ASN_Tools::Delete( asnMission.zone_a_prendre );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
