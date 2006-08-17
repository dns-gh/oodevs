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
#include "MIL_PionMission_Suivre.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Suivre::nDIAUniteASuivreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Suivre::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAUniteASuivreIdx_ = DEC_Tools::InitializeDIAField( "uniteASuivre_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Suivre::MIL_PionMission_Suivre( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Suivre::~MIL_PionMission_Suivre()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Suivre::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Suivre& asnMission = *asnMsg.mission.u.mission_pion_suivre;
    if( !NET_ASN_Tools::CopyAgent( asnMission.unite_a_suivre, GetVariable( nDIAUniteASuivreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Suivre::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Suivre::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Suivre& mission = static_cast< MIL_PionMission_Suivre& >( missionTmp );

    NET_ASN_Tools::CopyAgent( mission.GetVariable( nDIAUniteASuivreIdx_ ), GetVariable( nDIAUniteASuivreIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Suivre::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Suivre::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Suivre& asnMission = *new ASN1T_Mission_Pion_Suivre();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_suivre;
    asnMsg.mission.u.mission_pion_suivre  = &asnMission;

    NET_ASN_Tools::CopyAgent( GetVariable( nDIAUniteASuivreIdx_ ), asnMission.unite_a_suivre );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Suivre::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Suivre::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_suivre );
    ASN1T_Mission_Pion_Suivre& asnMission = *asnMsg.mission.u.mission_pion_suivre;


    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
