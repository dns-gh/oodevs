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
#include "MIL_PionMission_ABC_EscorterUnite.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_EscorterUnite::nDIAUniteAEscorterIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_EscorterUnite::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteAEscorterIdx_ = DEC_Tools::InitializeDIAField( "uniteAEscorter_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_EscorterUnite::MIL_PionMission_ABC_EscorterUnite( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_EscorterUnite::~MIL_PionMission_ABC_EscorterUnite()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_EscorterUnite::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_EscorterUnite& asnMission = *asnMsg.mission.u.mission_pion_abc_escorter_unite;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_escorter, GetVariable( nDIAUniteAEscorterIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_EscorterUnite::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteAEscorterIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_EscorterUnite::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_EscorterUnite& mission = static_cast< MIL_PionMission_ABC_EscorterUnite& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteAEscorterIdx_ ), GetVariable( nDIAUniteAEscorterIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_EscorterUnite::Terminate()
{
    NET_ASN_Tools::ResetAgent( GetVariable( nDIAUniteAEscorterIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_EscorterUnite::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_EscorterUnite& asnMission = *new ASN1T_Mission_Pion_ABC_EscorterUnite();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_escorter_unite;
    asnMsg.mission.u.mission_pion_abc_escorter_unite  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteAEscorterIdx_ ), asnMission.unite_a_escorter );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_EscorterUnite::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_EscorterUnite::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_escorter_unite );
    ASN1T_Mission_Pion_ABC_EscorterUnite& asnMission = *asnMsg.mission.u.mission_pion_abc_escorter_unite;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
