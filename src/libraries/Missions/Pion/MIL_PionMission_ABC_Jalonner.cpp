// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ABC_Jalonner.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ABC_Jalonner::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ABC_Jalonner::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Jalonner::MIL_PionMission_ABC_Jalonner( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_ABC_Jalonner::~MIL_PionMission_ABC_Jalonner()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ABC_Jalonner::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ABC_Jalonner& asnMission = *asnMsg.mission.u.mission_pion_abc_jalonner;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Jalonner::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_ABC_Jalonner::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ABC_Jalonner& mission = static_cast< MIL_PionMission_ABC_Jalonner& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Jalonner::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Jalonner::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ABC_Jalonner& asnMission = *new ASN1T_Mission_Pion_ABC_Jalonner();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_abc_jalonner;
    asnMsg.mission.u.mission_pion_abc_jalonner  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC_Jalonner::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_ABC_Jalonner::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_abc_jalonner );
    ASN1T_Mission_Pion_ABC_Jalonner& asnMission = *asnMsg.mission.u.mission_pion_abc_jalonner;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
