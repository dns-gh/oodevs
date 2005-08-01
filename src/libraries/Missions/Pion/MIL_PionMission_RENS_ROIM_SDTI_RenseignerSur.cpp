// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::nDIAFicelleDeVoleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAFicelleDeVoleIdx_ = DEC_Tools::InitializeDIAField( "ficelleDeVole_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::~MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_sdti_renseigner_sur;
    if( !NET_ASN_Tools::CopyPath( asnMission.ficelle_de_vole, ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur& mission = static_cast< MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAFicelleDeVoleIdx_ ), ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Terminate()
{
    NET_ASN_Tools::ResetPath( ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::Serialize
// Created: 2005-08-01 - 11:23:53
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
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_SDTI_RenseignerSur::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_rens_roim_sdti_renseigner_sur );
    ASN1T_Mission_Pion_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_sdti_renseigner_sur;

    NET_ASN_Tools::Delete( asnMission.ficelle_de_vole );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
