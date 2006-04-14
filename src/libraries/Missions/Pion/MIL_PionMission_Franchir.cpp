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
#include "MIL_PionMission_Franchir.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Franchir::nDIAPontFlottantIdx_ = 0 ;
int MIL_PionMission_Franchir::nDIAPointArriveeIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Franchir::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPontFlottantIdx_ = DEC_Tools::InitializeDIAField( "pontFlottant_", diaType );
    nDIAPointArriveeIdx_ = DEC_Tools::InitializeDIAField( "pointArrivee_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Franchir::MIL_PionMission_Franchir( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Franchir::~MIL_PionMission_Franchir()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Franchir::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Franchir& asnMission = *asnMsg.mission.u.mission_pion_franchir;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission.pont_flottant, GetVariable( nDIAPontFlottantIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_arrivee, GetVariable( nDIAPointArriveeIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Franchir::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Franchir::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Franchir& mission = static_cast< MIL_PionMission_Franchir& >( missionTmp );

    NET_ASN_Tools::CopyObjectKnowledge( mission.GetVariable( nDIAPontFlottantIdx_ ), GetVariable( nDIAPontFlottantIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointArriveeIdx_ ), GetVariable( nDIAPointArriveeIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Franchir::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Franchir::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Franchir& asnMission = *new ASN1T_Mission_Pion_Franchir();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_franchir;
    asnMsg.mission.u.mission_pion_franchir  = &asnMission;

    NET_ASN_Tools::CopyObjectKnowledge( GetVariable( nDIAPontFlottantIdx_ ), asnMission.pont_flottant, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointArriveeIdx_ ), asnMission.point_arrivee );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Franchir::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Franchir::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_franchir );
    ASN1T_Mission_Pion_Franchir& asnMission = *asnMsg.mission.u.mission_pion_franchir;

    NET_ASN_Tools::Delete( asnMission.point_arrivee );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
