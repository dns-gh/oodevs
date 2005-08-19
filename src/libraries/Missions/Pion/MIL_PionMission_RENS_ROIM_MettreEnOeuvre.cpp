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
#include "MIL_PionMission_RENS_ROIM_MettreEnOeuvre.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_RENS_ROIM_MettreEnOeuvre::nDIAPositionDeploiementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_RENS_ROIM_MettreEnOeuvre::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_MettreEnOeuvre::MIL_PionMission_RENS_ROIM_MettreEnOeuvre( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_RENS_ROIM_MettreEnOeuvre::~MIL_PionMission_RENS_ROIM_MettreEnOeuvre()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_mettre_en_oeuvre;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, positionDeploiement_, GetVariable( nDIAPositionDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( positionDeploiement_, GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_RENS_ROIM_MettreEnOeuvre& mission = static_cast< MIL_PionMission_RENS_ROIM_MettreEnOeuvre& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPositionDeploiementIdx_ ), positionDeploiement_, GetVariable( nDIAPositionDeploiementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Terminate()
{
    NET_ASN_Tools::ResetPoint( positionDeploiement_, GetVariable( nDIAPositionDeploiementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_MettreEnOeuvre::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *new ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre;
    asnMsg.mission.u.mission_pion_rens_roim_mettre_en_oeuvre  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_RENS_ROIM_MettreEnOeuvre::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_RENS_ROIM_MettreEnOeuvre::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_rens_roim_mettre_en_oeuvre );
    ASN1T_Mission_Pion_RENS_ROIM_MettreEnOeuvre& asnMission = *asnMsg.mission.u.mission_pion_rens_roim_mettre_en_oeuvre;

    NET_ASN_Tools::Delete( asnMission.position_deploiement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
