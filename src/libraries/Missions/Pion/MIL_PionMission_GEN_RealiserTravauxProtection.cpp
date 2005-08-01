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
#include "MIL_PionMission_GEN_RealiserTravauxProtection.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_GEN_RealiserTravauxProtection::nDIAPosObjetIdx_ = 0 ;
int MIL_PionMission_GEN_RealiserTravauxProtection::nDIAOidObjetPlanifieIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::InitializeDIA
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_GEN_RealiserTravauxProtection::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPosObjetIdx_ = DEC_Tools::InitializeDIAField( "posObjet_", diaType );
    nDIAOidObjetPlanifieIdx_ = DEC_Tools::InitializeDIAField( "oidObjetPlanifie_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxProtection::MIL_PionMission_GEN_RealiserTravauxProtection( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_PionMission_GEN_RealiserTravauxProtection::~MIL_PionMission_GEN_RealiserTravauxProtection()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection;
    if( !NET_ASN_Tools::CopyPoint( asnMission.pos_objet, posObjet_, GetVariable( nDIAPosObjetIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyID( asnMission.oid_objet_planifie, GetVariable( nDIAOidObjetPlanifieIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( posObjet_, GetVariable( nDIAPosObjetIdx_ ) );
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObjetPlanifieIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Initialize
// Created: 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
bool MIL_PionMission_GEN_RealiserTravauxProtection::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_GEN_RealiserTravauxProtection& mission = static_cast< MIL_PionMission_GEN_RealiserTravauxProtection& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPosObjetIdx_ ), posObjet_, GetVariable( nDIAPosObjetIdx_ ) );
    NET_ASN_Tools::CopyID( mission.GetVariable( nDIAOidObjetPlanifieIdx_ ), GetVariable( nDIAOidObjetPlanifieIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxProtection::Terminate()
{
    NET_ASN_Tools::ResetID( GetVariable( nDIAOidObjetPlanifieIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxProtection::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *new ASN1T_Mission_Pion_GEN_RealiserTravauxProtection();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_gen_realiser_travaux_protection;
    asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPosObjetIdx_ ), asnMission.pos_objet );
    NET_ASN_Tools::CopyID( GetVariable( nDIAOidObjetPlanifieIdx_ ), asnMission.oid_objet_planifie );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_GEN_RealiserTravauxProtection::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_PionMission_GEN_RealiserTravauxProtection::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_gen_realiser_travaux_protection );
    ASN1T_Mission_Pion_GEN_RealiserTravauxProtection& asnMission = *asnMsg.mission.u.mission_pion_gen_realiser_travaux_protection;

    NET_ASN_Tools::Delete( asnMission.pos_objet );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
