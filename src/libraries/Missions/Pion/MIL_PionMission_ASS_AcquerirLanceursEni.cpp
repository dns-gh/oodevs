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
#include "MIL_PionMission_ASS_AcquerirLanceursEni.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASS_AcquerirLanceursEni::nDIAZonesAObserverIdx_ = 0 ;
int MIL_PionMission_ASS_AcquerirLanceursEni::nDIAZoneImplantationIdx_ = 0 ;
int MIL_PionMission_ASS_AcquerirLanceursEni::nDIAMenanceRoemIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASS_AcquerirLanceursEni::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZonesAObserverIdx_ = DEC_Tools::InitializeDIAField( "zonesAObserver_", diaType );
    nDIAZoneImplantationIdx_ = DEC_Tools::InitializeDIAField( "zoneImplantation_", diaType );
    nDIAMenanceRoemIdx_ = DEC_Tools::InitializeDIAField( "menanceRoem_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AcquerirLanceursEni::MIL_PionMission_ASS_AcquerirLanceursEni( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AcquerirLanceursEni::~MIL_PionMission_ASS_AcquerirLanceursEni()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASS_AcquerirLanceursEni::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *asnMsg.mission.u.mission_pion_ass_acquerir_lanceurs_eni;
    if( !NET_ASN_Tools::CopyPolygonList( asnMission.zones_a_observer, GetVariable( nDIAZonesAObserverIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.zone_implantation, GetVariable( nDIAZoneImplantationIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.menance_roem, GetVariable( nDIAMenanceRoemIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AcquerirLanceursEni::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygonList( GetVariable( nDIAZonesAObserverIdx_ ) );
    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIAZoneImplantationIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAMenanceRoemIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AcquerirLanceursEni::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASS_AcquerirLanceursEni& mission = static_cast< MIL_PionMission_ASS_AcquerirLanceursEni& >( missionTmp );

    NET_ASN_Tools::CopyPolygonList( mission.GetVariable( nDIAZonesAObserverIdx_ ), GetVariable( nDIAZonesAObserverIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAZoneImplantationIdx_ ), GetVariable( nDIAZoneImplantationIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAMenanceRoemIdx_ ), GetVariable( nDIAMenanceRoemIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AcquerirLanceursEni::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAMenanceRoemIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AcquerirLanceursEni::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *new ASN1T_Mission_Pion_ASS_AcquerirLanceursEni();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni;
    asnMsg.mission.u.mission_pion_ass_acquerir_lanceurs_eni  = &asnMission;

    NET_ASN_Tools::CopyPolygonList( GetVariable( nDIAZonesAObserverIdx_ ), asnMission.zones_a_observer );
    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAZoneImplantationIdx_ ), asnMission.zone_implantation, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAMenanceRoemIdx_ ), asnMission.menance_roem );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AcquerirLanceursEni::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AcquerirLanceursEni::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_ass_acquerir_lanceurs_eni );
    ASN1T_Mission_Pion_ASS_AcquerirLanceursEni& asnMission = *asnMsg.mission.u.mission_pion_ass_acquerir_lanceurs_eni;

    NET_ASN_Tools::Delete( asnMission.zones_a_observer );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
