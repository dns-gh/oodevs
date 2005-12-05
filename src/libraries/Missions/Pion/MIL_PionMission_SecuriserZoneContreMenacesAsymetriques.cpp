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
#include "MIL_PionMission_SecuriserZoneContreMenacesAsymetriques.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::MIL_PionMission_SecuriserZoneContreMenacesAsymetriques( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::~MIL_PionMission_SecuriserZoneContreMenacesAsymetriques()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques& asnMission = *asnMsg.mission.u.mission_pion_securiser_zone_contre_menaces_asymetriques;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, zone_, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_SecuriserZoneContreMenacesAsymetriques& mission = static_cast< MIL_PionMission_SecuriserZoneContreMenacesAsymetriques& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneIdx_ ), zone_, GetVariable( nDIAZoneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zone_, GetVariable( nDIAZoneIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques& asnMission = *new ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_securiser_zone_contre_menaces_asymetriques;
    asnMsg.mission.u.mission_pion_securiser_zone_contre_menaces_asymetriques  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_SecuriserZoneContreMenacesAsymetriques::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_securiser_zone_contre_menaces_asymetriques );
    ASN1T_Mission_Pion_SecuriserZoneContreMenacesAsymetriques& asnMission = *asnMsg.mission.u.mission_pion_securiser_zone_contre_menaces_asymetriques;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
