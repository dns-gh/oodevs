// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:44 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_INF_DetruireParCoupDeMain.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_INF_DetruireParCoupDeMain::nDIAObjectifIdx_ = 0 ;
int MIL_PionMission_INF_DetruireParCoupDeMain::nDIAPointRegroupementIdx_ = 0 ;
int MIL_PionMission_INF_DetruireParCoupDeMain::nDIAHeureDestructionIdx_ = 0 ;
int MIL_PionMission_INF_DetruireParCoupDeMain::nDIANatureDestructionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::InitializeDIA
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_DetruireParCoupDeMain::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAObjectifIdx_ = DEC_Tools::InitializeDIAField( "objectif_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );
    nDIAHeureDestructionIdx_ = DEC_Tools::InitializeDIAField( "heureDestruction_", diaType );
    nDIANatureDestructionIdx_ = DEC_Tools::InitializeDIAField( "natureDestruction_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain constructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_INF_DetruireParCoupDeMain::MIL_PionMission_INF_DetruireParCoupDeMain( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain destructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_INF_DetruireParCoupDeMain::~MIL_PionMission_INF_DetruireParCoupDeMain()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_DetruireParCoupDeMain::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *asnMsg.mission.u.mission_pion_inf_detruire_par_coup_de_main;
    if( !NET_ASN_Tools::CopyPoint( asnMission.objectif, objectif_, GetVariable( nDIAObjectifIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyGDH( asnMission.heure_destruction, GetVariable( nDIAHeureDestructionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.nature_destruction, GetVariable( nDIANatureDestructionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_DetruireParCoupDeMain::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( objectif_, GetVariable( nDIAObjectifIdx_ ) );
    NET_ASN_Tools::ResetPoint( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::ResetGDH( GetVariable( nDIAHeureDestructionIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIANatureDestructionIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::Initialize
// Created: 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_DetruireParCoupDeMain::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_DetruireParCoupDeMain& mission = static_cast< MIL_PionMission_INF_DetruireParCoupDeMain& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAObjectifIdx_ ), objectif_, GetVariable( nDIAObjectifIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointRegroupementIdx_ ), pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );
    NET_ASN_Tools::CopyGDH( mission.GetVariable( nDIAHeureDestructionIdx_ ), GetVariable( nDIAHeureDestructionIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIANatureDestructionIdx_ ), GetVariable( nDIANatureDestructionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::Terminate
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireParCoupDeMain::Terminate()
{
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIANatureDestructionIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::Serialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireParCoupDeMain::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *new ASN1T_Mission_Pion_INF_DetruireParCoupDeMain();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main;
    asnMsg.mission.u.mission_pion_inf_detruire_par_coup_de_main  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAObjectifIdx_ ), asnMission.objectif );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );
    NET_ASN_Tools::CopyGDH( GetVariable( nDIAHeureDestructionIdx_ ), asnMission.heure_destruction );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIANatureDestructionIdx_ ), asnMission.nature_destruction );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_DetruireParCoupDeMain::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_DetruireParCoupDeMain::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_detruire_par_coup_de_main );
    ASN1T_Mission_Pion_INF_DetruireParCoupDeMain& asnMission = *asnMsg.mission.u.mission_pion_inf_detruire_par_coup_de_main;

    NET_ASN_Tools::Delete( asnMission.objectif );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
