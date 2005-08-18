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

#include "MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::nDIAPositionsAReconnaitreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::~MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPositionsAReconnaitreIdx_ = DEC_Tools::InitializeDIAField( "positionsAReconnaitre_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *asnMsg.mission.u.mission_automate_ass_reconnaitre_zones_deploiement;
    if( !NET_ASN_Tools::CopyPointList( asnMission.positions_a_reconnaitre, positionsAReconnaitre_, GetVariable( nDIAPositionsAReconnaitreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::Terminate()
{
    NET_ASN_Tools::ResetPointList( positionsAReconnaitre_, GetVariable( nDIAPositionsAReconnaitreIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *new ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement;
    asnMsg.mission.u.mission_automate_ass_reconnaitre_zones_deploiement  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPositionsAReconnaitreIdx_ ), asnMission.positions_a_reconnaitre );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_ReconnaitreZonesDeploiement::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_ass_reconnaitre_zones_deploiement );
    ASN1T_Mission_Automate_ASS_ReconnaitreZonesDeploiement& asnMission = *asnMsg.mission.u.mission_automate_ass_reconnaitre_zones_deploiement;

    NET_ASN_Tools::Delete( asnMission.positions_a_reconnaitre );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
