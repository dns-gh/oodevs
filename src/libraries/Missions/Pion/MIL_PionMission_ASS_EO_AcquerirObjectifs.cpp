// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_ASS_EO_AcquerirObjectifs.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASS_EO_AcquerirObjectifs::nDIAZoneAObserverIdx_ = 0 ;
int MIL_PionMission_ASS_EO_AcquerirObjectifs::nDIAUniteAAppuyerIdx_ = 0 ;
int MIL_PionMission_ASS_EO_AcquerirObjectifs::nDIACategoriesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::InitializeDIA
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASS_EO_AcquerirObjectifs::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneAObserverIdx_ = DEC_Tools::InitializeDIAField( "zoneAObserver_", diaType );
    nDIAUniteAAppuyerIdx_ = DEC_Tools::InitializeDIAField( "uniteAAppuyer_", diaType );
    nDIACategoriesIdx_ = DEC_Tools::InitializeDIAField( "categories_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_EO_AcquerirObjectifs::MIL_PionMission_ASS_EO_AcquerirObjectifs( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASS_EO_AcquerirObjectifs::~MIL_PionMission_ASS_EO_AcquerirObjectifs()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *asnMsg.mission.u.mission_pion_ass_eo_acquerir_objectifs;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_a_observer, zoneAObserver_, GetVariable( nDIAZoneAObserverIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.unite_a_appuyer, GetVariable( nDIAUniteAAppuyerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNatureAtlas( asnMission.categories, GetVariable( nDIACategoriesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( zoneAObserver_, GetVariable( nDIAZoneAObserverIdx_ ) );
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIAUniteAAppuyerIdx_ ) );
    NET_ASN_Tools::ResetNatureAtlas( GetVariable( nDIACategoriesIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize
// Created: 2005-08-17 - 16:30:43
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASS_EO_AcquerirObjectifs::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASS_EO_AcquerirObjectifs& mission = static_cast< MIL_PionMission_ASS_EO_AcquerirObjectifs& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIAZoneAObserverIdx_ ), zoneAObserver_, GetVariable( nDIAZoneAObserverIdx_ ) );
    NET_ASN_Tools::CopyAutomate( mission.GetVariable( nDIAUniteAAppuyerIdx_ ), GetVariable( nDIAUniteAAppuyerIdx_ ) );
    NET_ASN_Tools::CopyNatureAtlas( mission.GetVariable( nDIACategoriesIdx_ ), GetVariable( nDIACategoriesIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_EO_AcquerirObjectifs::Terminate()
{
    NET_ASN_Tools::ResetNatureAtlas( GetVariable( nDIACategoriesIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_EO_AcquerirObjectifs::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *new ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs;
    asnMsg.mission.u.mission_pion_ass_eo_acquerir_objectifs  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneAObserverIdx_ ), asnMission.zone_a_observer );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIAUniteAAppuyerIdx_ ), asnMission.unite_a_appuyer );
    NET_ASN_Tools::CopyNatureAtlas( GetVariable( nDIACategoriesIdx_ ), asnMission.categories );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASS_EO_AcquerirObjectifs::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASS_EO_AcquerirObjectifs::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_ass_eo_acquerir_objectifs );
    ASN1T_Mission_Pion_ASS_EO_AcquerirObjectifs& asnMission = *asnMsg.mission.u.mission_pion_ass_eo_acquerir_objectifs;

    NET_ASN_Tools::Delete( asnMission.zone_a_observer );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
