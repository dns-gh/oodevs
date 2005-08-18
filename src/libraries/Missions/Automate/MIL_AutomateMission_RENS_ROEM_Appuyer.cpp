// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_RENS_ROEM_Appuyer.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_ROEM_Appuyer::nDIAPositionDeploiementIdx_ = 0 ;
int MIL_AutomateMission_RENS_ROEM_Appuyer::nDIAPointRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROEM_Appuyer::MIL_AutomateMission_RENS_ROEM_Appuyer( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROEM_Appuyer::~MIL_AutomateMission_RENS_ROEM_Appuyer()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_ROEM_Appuyer::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionDeploiement_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_ROEM_Appuyer::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_ROEM_Appuyer& asnMission = *asnMsg.mission.u.mission_automate_rens_roem_appuyer;
    if( !NET_ASN_Tools::CopyPoint( asnMission.position_deploiement, positionDeploiement_, GetVariable( nDIAPositionDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROEM_Appuyer::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROEM_Appuyer::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_ROEM_Appuyer& asnMission = *new ASN1T_Mission_Automate_RENS_ROEM_Appuyer();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_roem_appuyer;
    asnMsg.mission.u.mission_automate_rens_roem_appuyer  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPositionDeploiementIdx_ ), asnMission.position_deploiement );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROEM_Appuyer::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROEM_Appuyer::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_roem_appuyer );
    ASN1T_Mission_Automate_RENS_ROEM_Appuyer& asnMission = *asnMsg.mission.u.mission_automate_rens_roem_appuyer;

    NET_ASN_Tools::Delete( asnMission.position_deploiement );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
