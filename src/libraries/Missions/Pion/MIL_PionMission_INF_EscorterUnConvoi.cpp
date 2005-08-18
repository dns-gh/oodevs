// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:44 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_INF_EscorterUnConvoi.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_EscorterUnConvoi::nDIAConvoiAEscorterIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::InitializeDIA
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_EscorterUnConvoi::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAConvoiAEscorterIdx_ = DEC_Tools::InitializeDIAField( "convoiAEscorter_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi constructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_INF_EscorterUnConvoi::MIL_PionMission_INF_EscorterUnConvoi( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi destructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_INF_EscorterUnConvoi::~MIL_PionMission_INF_EscorterUnConvoi()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_EscorterUnConvoi::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_EscorterUnConvoi& asnMission = *asnMsg.mission.u.mission_pion_inf_escorter_un_convoi;
    if( !NET_ASN_Tools::CopyAgent( asnMission.convoi_a_escorter, GetVariable( nDIAConvoiAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_EscorterUnConvoi::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAConvoiAEscorterIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::Initialize
// Created: 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_EscorterUnConvoi::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_EscorterUnConvoi& mission = static_cast< MIL_PionMission_INF_EscorterUnConvoi& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAConvoiAEscorterIdx_ ), GetVariable( nDIAConvoiAEscorterIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::Terminate
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_EscorterUnConvoi::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAConvoiAEscorterIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::Serialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_EscorterUnConvoi::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_EscorterUnConvoi& asnMission = *new ASN1T_Mission_Pion_INF_EscorterUnConvoi();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_escorter_un_convoi;
    asnMsg.mission.u.mission_pion_inf_escorter_un_convoi  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAConvoiAEscorterIdx_ ), asnMission.convoi_a_escorter );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_EscorterUnConvoi::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_EscorterUnConvoi::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_escorter_un_convoi );
    ASN1T_Mission_Pion_INF_EscorterUnConvoi& asnMission = *asnMsg.mission.u.mission_pion_inf_escorter_un_convoi;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
