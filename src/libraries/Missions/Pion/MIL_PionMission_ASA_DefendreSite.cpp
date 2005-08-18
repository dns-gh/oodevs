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
#include "MIL_PionMission_ASA_DefendreSite.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_ASA_DefendreSite::nDIASiteIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::InitializeDIA
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_ASA_DefendreSite::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASiteIdx_ = DEC_Tools::InitializeDIAField( "site_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreSite::MIL_PionMission_ASA_DefendreSite( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_PionMission_ASA_DefendreSite::~MIL_PionMission_ASA_DefendreSite()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_ASA_DefendreSite::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_site;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.site, site_, GetVariable( nDIASiteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreSite::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPolygon( site_, GetVariable( nDIASiteIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::Initialize
// Created: 2005-08-17 - 16:30:43
// -----------------------------------------------------------------------------
bool MIL_PionMission_ASA_DefendreSite::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_ASA_DefendreSite& mission = static_cast< MIL_PionMission_ASA_DefendreSite& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIASiteIdx_ ), site_, GetVariable( nDIASiteIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreSite::Terminate()
{
    NET_ASN_Tools::ResetPolygon( site_, GetVariable( nDIASiteIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreSite::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *new ASN1T_Mission_Pion_ASA_DefendreSite();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_asa_defendre_site;
    asnMsg.mission.u.mission_pion_asa_defendre_site  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASiteIdx_ ), asnMission.site );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_ASA_DefendreSite::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_PionMission_ASA_DefendreSite::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_asa_defendre_site );
    ASN1T_Mission_Pion_ASA_DefendreSite& asnMission = *asnMsg.mission.u.mission_pion_asa_defendre_site;

    NET_ASN_Tools::Delete( asnMission.site );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
