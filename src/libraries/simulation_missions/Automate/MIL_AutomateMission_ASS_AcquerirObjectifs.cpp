// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_ASS_AcquerirObjectifs.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASS_AcquerirObjectifs::nDIAZonesAObserverIdx_ = 0 ;
int MIL_AutomateMission_ASS_AcquerirObjectifs::nDIAPositionsDeploiementIdx_ = 0 ;
int MIL_AutomateMission_ASS_AcquerirObjectifs::nDIACategoriesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_AcquerirObjectifs::MIL_AutomateMission_ASS_AcquerirObjectifs( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASS_AcquerirObjectifs::~MIL_AutomateMission_ASS_AcquerirObjectifs()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASS_AcquerirObjectifs::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZonesAObserverIdx_ = DEC_Tools::InitializeDIAField( "zonesAObserver_", diaType );
    nDIAPositionsDeploiementIdx_ = DEC_Tools::InitializeDIAField( "positionsDeploiement_", diaType );
    nDIACategoriesIdx_ = DEC_Tools::InitializeDIAField( "categories_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASS_AcquerirObjectifs::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *asnMsg.mission.u.mission_automate_ass_acquerir_objectifs;
    if( !NET_ASN_Tools::CopyPolygonList( asnMission.zones_a_observer, GetVariable( nDIAZonesAObserverIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPointList( asnMission.positions_deploiement, GetVariable( nDIAPositionsDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.categories, GetVariable( nDIACategoriesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_AcquerirObjectifs::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Automate_ASS_AcquerirObjectifs();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_ass_acquerir_objectifs;
    asnMsg.mission.u.mission_automate_ass_acquerir_objectifs  = &asnMission;

    NET_ASN_Tools::CopyPolygonList( GetVariable( nDIAZonesAObserverIdx_ ), asnMission.zones_a_observer );
    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPositionsDeploiementIdx_ ), asnMission.positions_deploiement );
    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACategoriesIdx_ ), asnMission.categories );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASS_AcquerirObjectifs::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASS_AcquerirObjectifs::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_ass_acquerir_objectifs );
    ASN1T_Mission_Automate_ASS_AcquerirObjectifs& asnMission = *asnMsg.mission.u.mission_automate_ass_acquerir_objectifs;

    NET_ASN_Tools::Delete( asnMission.zones_a_observer );
    NET_ASN_Tools::Delete( asnMission.positions_deploiement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
