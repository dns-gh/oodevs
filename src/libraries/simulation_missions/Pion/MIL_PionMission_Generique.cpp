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
#include "MIL_PionMission_Generique.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_Generique::nDIAZonesIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAPointsIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAItinerairesIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAUnitesIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAAutomatesIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAConnaissancesAgentsIdx_ = 0 ;
int MIL_PionMission_Generique::nDIAConnaissancesObjetsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Generique::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZonesIdx_ = DEC_Tools::InitializeDIAField( "zones_", diaType );
    nDIAPointsIdx_ = DEC_Tools::InitializeDIAField( "points_", diaType );
    nDIAItinerairesIdx_ = DEC_Tools::InitializeDIAField( "itineraires_", diaType );
    nDIAUnitesIdx_ = DEC_Tools::InitializeDIAField( "unites_", diaType );
    nDIAAutomatesIdx_ = DEC_Tools::InitializeDIAField( "automates_", diaType );
    nDIAConnaissancesAgentsIdx_ = DEC_Tools::InitializeDIAField( "connaissancesAgents_", diaType );
    nDIAConnaissancesObjetsIdx_ = DEC_Tools::InitializeDIAField( "connaissancesObjets_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Generique::MIL_PionMission_Generique( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_Generique::~MIL_PionMission_Generique()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Generique::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Generique& asnMission = *asnMsg.mission.u.mission_pion_generique;
    if( !NET_ASN_Tools::CopyPolygonList( asnMission.zones, GetVariable( nDIAZonesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points, GetVariable( nDIAPointsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPathList( asnMission.itineraires, GetVariable( nDIAItinerairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites, GetVariable( nDIAUnitesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomateList( asnMission.automates, GetVariable( nDIAAutomatesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentKnowledgeList( asnMission.connaissances_agents, GetVariable( nDIAConnaissancesAgentsIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.connaissances_objets, GetVariable( nDIAConnaissancesObjetsIdx_ ), pion_.GetKnowledgeGroup() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_Generique::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_Generique::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Generique& mission = static_cast< MIL_PionMission_Generique& >( missionTmp );

    NET_ASN_Tools::CopyPolygonList( mission.GetVariable( nDIAZonesIdx_ ), GetVariable( nDIAZonesIdx_ ) );
    NET_ASN_Tools::CopyPointList( mission.GetVariable( nDIAPointsIdx_ ), GetVariable( nDIAPointsIdx_ ) );
    NET_ASN_Tools::CopyPathList( mission.GetVariable( nDIAItinerairesIdx_ ), GetVariable( nDIAItinerairesIdx_ ) );
    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUnitesIdx_ ), GetVariable( nDIAUnitesIdx_ ) );
    NET_ASN_Tools::CopyAutomateList( mission.GetVariable( nDIAAutomatesIdx_ ), GetVariable( nDIAAutomatesIdx_ ) );
    NET_ASN_Tools::CopyAgentKnowledgeList( mission.GetVariable( nDIAConnaissancesAgentsIdx_ ), GetVariable( nDIAConnaissancesAgentsIdx_ ) );
    NET_ASN_Tools::CopyObjectKnowledgeList( mission.GetVariable( nDIAConnaissancesObjetsIdx_ ), GetVariable( nDIAConnaissancesObjetsIdx_ ) );

    return true;
}                                                                    

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Generique::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Generique& asnMission = *new ASN1T_Mission_Pion_Generique();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_generique;
    asnMsg.mission.u.mission_pion_generique  = &asnMission;

    NET_ASN_Tools::CopyPolygonList( GetVariable( nDIAZonesIdx_ ), asnMission.zones );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsIdx_ ), asnMission.points );
    NET_ASN_Tools::CopyPathList( GetVariable( nDIAItinerairesIdx_ ), asnMission.itineraires );
    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesIdx_ ), asnMission.unites );
    NET_ASN_Tools::CopyAutomateList( GetVariable( nDIAAutomatesIdx_ ), asnMission.automates );
    NET_ASN_Tools::CopyAgentKnowledgeList( GetVariable( nDIAConnaissancesAgentsIdx_ ), asnMission.connaissances_agents, pion_.GetKnowledgeGroup() );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAConnaissancesObjetsIdx_ ), asnMission.connaissances_objets, pion_.GetKnowledgeGroup() );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Generique::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_Generique::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_generique );
    ASN1T_Mission_Pion_Generique& asnMission = *asnMsg.mission.u.mission_pion_generique;

    NET_ASN_Tools::Delete( asnMission.zones );
    NET_ASN_Tools::Delete( asnMission.points );
    NET_ASN_Tools::Delete( asnMission.itineraires );
    NET_ASN_Tools::Delete( asnMission.unites );
    NET_ASN_Tools::Delete( asnMission.automates );
    NET_ASN_Tools::Delete( asnMission.connaissances_agents );
    NET_ASN_Tools::Delete( asnMission.connaissances_objets );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
