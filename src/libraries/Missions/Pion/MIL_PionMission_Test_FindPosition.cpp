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
#include "MIL_PionMission_Test_FindPosition.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_FindPosition::nDIAPointIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIARetreatIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIAUniteIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIATypeObjectifIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIATypeOperationIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIAAvoidIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIARecoIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIANoObstacleAvoidIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIADebordementIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIADecPointsIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIANiveauIdentificationIdx_ = 0 ;
int MIL_PionMission_Test_FindPosition::nDIADgoIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_FindPosition::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointIdx_ = DEC_Tools::InitializeDIAField( "point_", diaType );
    nDIARetreatIdx_ = DEC_Tools::InitializeDIAField( "retreat_", diaType );
    nDIAUniteIdx_ = DEC_Tools::InitializeDIAField( "unite_", diaType );
    nDIATypeObjectifIdx_ = DEC_Tools::InitializeDIAField( "typeObjectif_", diaType );
    nDIATypeOperationIdx_ = DEC_Tools::InitializeDIAField( "typeOperation_", diaType );
    nDIAAvoidIdx_ = DEC_Tools::InitializeDIAField( "avoid_", diaType );
    nDIARecoIdx_ = DEC_Tools::InitializeDIAField( "reco_", diaType );
    nDIANoObstacleAvoidIdx_ = DEC_Tools::InitializeDIAField( "noObstacleAvoid_", diaType );
    nDIADebordementIdx_ = DEC_Tools::InitializeDIAField( "debordement_", diaType );
    nDIADecPointsIdx_ = DEC_Tools::InitializeDIAField( "decPoints_", diaType );
    nDIANiveauIdentificationIdx_ = DEC_Tools::InitializeDIAField( "niveauIdentification_", diaType );
    nDIADgoIdx_ = DEC_Tools::InitializeDIAField( "dgo_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_FindPosition::MIL_PionMission_Test_FindPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PionMission_Test_FindPosition::~MIL_PionMission_Test_FindPosition()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_FindPosition::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_FindPosition& asnMission = *asnMsg.mission.u.mission_pion_test_find_position;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point, point_, GetVariable( nDIAPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.retreat, retreat_, GetVariable( nDIARetreatIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgentList( asnMission.unite, GetVariable( nDIAUniteIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_objectif, GetVariable( nDIATypeObjectifIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_operation, GetVariable( nDIATypeOperationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.avoid, GetVariable( nDIAAvoidIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.reco, GetVariable( nDIARecoIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.no_obstacle_avoid, GetVariable( nDIANoObstacleAvoidIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.debordement, GetVariable( nDIADebordementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.dec_points, GetVariable( nDIADecPointsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.niveau_identification, GetVariable( nDIANiveauIdentificationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyDirection( asnMission.dgo, dgo_, GetVariable( nDIADgoIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_FindPosition::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( point_, GetVariable( nDIAPointIdx_ ) );
    NET_ASN_Tools::ResetPoint( retreat_, GetVariable( nDIARetreatIdx_ ) );
    NET_ASN_Tools::ResetAgentList( GetVariable( nDIAUniteIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeObjectifIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeOperationIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIAAvoidIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIARecoIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIANoObstacleAvoidIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIADebordementIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIADecPointsIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIANiveauIdentificationIdx_ ) );
    NET_ASN_Tools::ResetDirection( dgo_, GetVariable( nDIADgoIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::Initialize
// Created: 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_FindPosition::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_FindPosition& mission = static_cast< MIL_PionMission_Test_FindPosition& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointIdx_ ), point_, GetVariable( nDIAPointIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIARetreatIdx_ ), retreat_, GetVariable( nDIARetreatIdx_ ) );
    NET_ASN_Tools::CopyAgentList( mission.GetVariable( nDIAUniteIdx_ ), GetVariable( nDIAUniteIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeObjectifIdx_ ), GetVariable( nDIATypeObjectifIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeOperationIdx_ ), GetVariable( nDIATypeOperationIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAAvoidIdx_ ), GetVariable( nDIAAvoidIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIARecoIdx_ ), GetVariable( nDIARecoIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIANoObstacleAvoidIdx_ ), GetVariable( nDIANoObstacleAvoidIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIADebordementIdx_ ), GetVariable( nDIADebordementIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIADecPointsIdx_ ), GetVariable( nDIADecPointsIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIANiveauIdentificationIdx_ ), GetVariable( nDIANiveauIdentificationIdx_ ) );
    NET_ASN_Tools::CopyDirection( mission.GetVariable( nDIADgoIdx_ ), dgo_, GetVariable( nDIADgoIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_FindPosition::Terminate()
{
    NET_ASN_Tools::ResetDirection( dgo_, GetVariable( nDIADgoIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::Serialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_FindPosition::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *new ASN1T_Mission_Pion_Test_FindPosition();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_find_position;
    asnMsg.mission.u.mission_pion_test_find_position  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointIdx_ ), asnMission.point );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIARetreatIdx_ ), asnMission.retreat );
    NET_ASN_Tools::CopyAgentList( GetVariable( nDIAUniteIdx_ ), asnMission.unite );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeObjectifIdx_ ), asnMission.type_objectif );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeOperationIdx_ ), asnMission.type_operation );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAAvoidIdx_ ), asnMission.avoid );
    NET_ASN_Tools::CopyBool( GetVariable( nDIARecoIdx_ ), asnMission.reco );
    NET_ASN_Tools::CopyBool( GetVariable( nDIANoObstacleAvoidIdx_ ), asnMission.no_obstacle_avoid );
    NET_ASN_Tools::CopyBool( GetVariable( nDIADebordementIdx_ ), asnMission.debordement );
    NET_ASN_Tools::CopyBool( GetVariable( nDIADecPointsIdx_ ), asnMission.dec_points );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIANiveauIdentificationIdx_ ), asnMission.niveau_identification );
    NET_ASN_Tools::CopyDirection( GetVariable( nDIADgoIdx_ ), asnMission.dgo );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_FindPosition::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_FindPosition::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_find_position );
    ASN1T_Mission_Pion_Test_FindPosition& asnMission = *asnMsg.mission.u.mission_pion_test_find_position;

    NET_ASN_Tools::Delete( asnMission.point );
    NET_ASN_Tools::Delete( asnMission.retreat );
    NET_ASN_Tools::Delete( asnMission.unite );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
