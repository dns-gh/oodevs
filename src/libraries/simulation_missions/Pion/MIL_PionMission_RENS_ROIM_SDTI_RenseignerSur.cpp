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
#include "MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::nDIAFicelleDeVoleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAFicelleDeVoleIdx_ = DEC_Tools::InitializeDIAField( "ficelleDeVole_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::~MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_sdti_renseigner_sur;
    if( !NET_ASN_Tools::CopyPath( asnMission.ficelle_de_vole, GetVariable( nDIAFicelleDeVoleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur& mission = static_cast< MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAFicelleDeVoleIdx_ ), GetVariable( nDIAFicelleDeVoleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur;
    asnMsg.mission.u.mission_pion_rens_roim_sdti_renseigner_sur  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAFicelleDeVoleIdx_ ), asnMission.ficelle_de_vole );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur );
    ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_sdti_renseigner_sur;

    NET_ASN_Tools::Delete( asnMission.ficelle_de_vole );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
