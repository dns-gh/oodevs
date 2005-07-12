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
#include "MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::nDIAPositionImplantationIdx_ = 0 ;
int MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::nDIAPresenceContreBatterieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::InitializeDIA
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionImplantationIdx_ = DEC_Tools::InitializeDIAField( "positionImplantation_", diaType );
    nDIAPresenceContreBatterieIdx_ = DEC_Tools::InitializeDIAField( "presenceContreBatterie_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::~MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_implantation, positionImplantation_, GetVariable( nDIAPositionImplantationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.presence_contre_batterie, GetVariable( nDIAPresenceContreBatterieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( positionImplantation_, GetVariable( nDIAPositionImplantationIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize
// Created: 2005-6-28 - 14:3:3
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition& mission = static_cast< MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionImplantationIdx_ ), positionImplantation_, GetVariable( nDIAPositionImplantationIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAPresenceContreBatterieIdx_ ), GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Terminate()
{
    NET_ASN_Tools::ResetBool( GetVariable( nDIAPresenceContreBatterieIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *new ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position;
    asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionImplantationIdx_ ), asnMission.position_implantation );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPresenceContreBatterieIdx_ ), asnMission.presence_contre_batterie );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_AssurerMiseEnOeuvreSurPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_ass_assurer_mise_en_oeuvre_sur_position );
    ASN1T_Mission_Pion_ASS_AssurerMiseEnOeuvreSurPosition& asnMission = *asnMsg.mission.u.mission_pion_ass_assurer_mise_en_oeuvre_sur_position;

    NET_ASN_Tools::Delete( asnMission.position_implantation );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
