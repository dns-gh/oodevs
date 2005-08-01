// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_Generique.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_Generique::nDIAZonesIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAPointsIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAItinerairesIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAUnitesIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAAutomatesIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAConnaissancesAgentsIdx_ = 0 ;
int MIL_AutomateMission_Generique::nDIAConnaissancesObjetsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Generique constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_Generique::MIL_AutomateMission_Generique( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Generique destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_Generique::~MIL_AutomateMission_Generique()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Generique::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_Generique::InitializeDIA( const MIL_AutomateMissionType& type )
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
// Name: MIL_AutomateMission_Generique::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_Generique::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_Generique& asnMission = *asnMsg.mission.u.mission_automate_generique;
    if( !NET_ASN_Tools::CopyPolygonList( asnMission.zones, GetVariable( nDIAZonesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points, points_, GetVariable( nDIAPointsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPathList( asnMission.itineraires, GetVariable( nDIAItinerairesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unites, GetVariable( nDIAUnitesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomateList( asnMission.automates, GetVariable( nDIAAutomatesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentKnowledgeList( asnMission.connaissances_agents, GetVariable( nDIAConnaissancesAgentsIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyObjectKnowledgeList( asnMission.connaissances_objets, GetVariable( nDIAConnaissancesObjetsIdx_ ), automate_.GetKnowledgeGroup().GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Generique::Terminate()
{
    NET_ASN_Tools::ResetObjectKnowledgeList( GetVariable( nDIAConnaissancesObjetsIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Generique::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Generique::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_Generique& asnMission = *new ASN1T_Mission_Automate_Generique();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_generique;
    asnMsg.mission.u.mission_automate_generique  = &asnMission;

    NET_ASN_Tools::CopyPolygonList( GetVariable( nDIAZonesIdx_ ), asnMission.zones );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsIdx_ ), asnMission.points );
    NET_ASN_Tools::CopyPathList( GetVariable( nDIAItinerairesIdx_ ), asnMission.itineraires );
    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUnitesIdx_ ), asnMission.unites );
    NET_ASN_Tools::CopyAutomateList( GetVariable( nDIAAutomatesIdx_ ), asnMission.automates );
    NET_ASN_Tools::CopyAgentKnowledgeList( GetVariable( nDIAConnaissancesAgentsIdx_ ), asnMission.connaissances_agents, automate_.GetKnowledgeGroup().GetKSQuerier() );
    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( nDIAConnaissancesObjetsIdx_ ), asnMission.connaissances_objets, automate_.GetKnowledgeGroup().GetKSQuerier() );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_Generique::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_Generique::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_generique );
    ASN1T_Mission_Automate_Generique& asnMission = *asnMsg.mission.u.mission_automate_generique;

    NET_ASN_Tools::Delete( asnMission.connaissances_agents );
    NET_ASN_Tools::Delete( asnMission.connaissances_objets );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
