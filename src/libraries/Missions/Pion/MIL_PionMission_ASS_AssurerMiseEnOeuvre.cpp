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
#include "MIL_PionMission_ASS_AssurerMiseEnOeuvre.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASS_AssurerMiseEnOeuvre::nDIAZoneImplantationIdx_ = 0 ;
int MIL_PionMission_ASS_AssurerMiseEnOeuvre::nDIAPresenceContreBatterieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASS_AssurerMiseEnOeuvre::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneImplantationIdx_ = DEC_Tools::InitializeDIAField( "zoneImplantation_", diaType );
    nDIAPresenceContreBatterieIdx_ = DEC_Tools::InitializeDIAField( "presenceContreBatterie_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AssurerMiseEnOeuvre::MIL_PionMission_ASS_AssurerMiseEnOeuvre( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AssurerMiseEnOeuvre::~MIL_PionMission_ASS_AssurerMiseEnOeuvre()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.zone_implantation, GetVariable( nDIAZoneImplantationIdx_ ), pion_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.presence_contre_batterie, GetVariable( nDIAPresenceContreBatterieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetObjectKnowledge( GetVariable( nDIAZoneImplantationIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AssurerMiseEnOeuvre::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASS_AssurerMiseEnOeuvre& mission = static_cast< MIL_PionMission_ASS_AssurerMiseEnOeuvre& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAZoneImplantationIdx_ ), GetVariable( nDIAZoneImplantationIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAPresenceContreBatterieIdx_ ), GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvre::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvre::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre;
    asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAZoneImplantationIdx_ ), asnMission.zone_implantation, pion_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPresenceContreBatterieIdx_ ), asnMission.presence_contre_batterie );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvre::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvre::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre );
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvre& asnMission = *asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
