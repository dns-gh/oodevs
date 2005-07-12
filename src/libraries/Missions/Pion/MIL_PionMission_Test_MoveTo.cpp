// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:4 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PionMission_Test_MoveTo.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_Test_MoveTo::nDIAItineraireIdx_ = 0 ;
int MIL_PionMission_Test_MoveTo::nDIATypeItineraireIdx_ = 0 ;
int MIL_PionMission_Test_MoveTo::nDIADebarqueIdx_ = 0 ;
int MIL_PionMission_Test_MoveTo::nDIAVerrouillageVisionIdx_ = 0 ;
int MIL_PionMission_Test_MoveTo::nDIAVisionPointIdx_ = 0 ;
int MIL_PionMission_Test_MoveTo::nDIAVisionDirectionIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::InitializeDIA
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_Test_MoveTo::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );
    nDIATypeItineraireIdx_ = DEC_Tools::InitializeDIAField( "typeItineraire_", diaType );
    nDIADebarqueIdx_ = DEC_Tools::InitializeDIAField( "debarque_", diaType );
    nDIAVerrouillageVisionIdx_ = DEC_Tools::InitializeDIAField( "verrouillageVision_", diaType );
    nDIAVisionPointIdx_ = DEC_Tools::InitializeDIAField( "visionPoint_", diaType );
    nDIAVisionDirectionIdx_ = DEC_Tools::InitializeDIAField( "visionDirection_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo constructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_Test_MoveTo::MIL_PionMission_Test_MoveTo( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo destructor
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_PionMission_Test_MoveTo::~MIL_PionMission_Test_MoveTo()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::Initialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_Test_MoveTo::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_Test_MoveTo& asnMission = *asnMsg.mission.u.mission_pion_test_move_to;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.type_itineraire, GetVariable( nDIATypeItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.debarque, GetVariable( nDIADebarqueIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.verrouillage_vision, GetVariable( nDIAVerrouillageVisionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPoint( asnMission.vision_point, visionPoint_, GetVariable( nDIAVisionPointIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyDirection( asnMission.vision_direction, visionDirection_, GetVariable( nDIAVisionDirectionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::Initialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
bool MIL_PionMission_Test_MoveTo::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIATypeItineraireIdx_ ) );
    NET_ASN_Tools::ResetBool( GetVariable( nDIADebarqueIdx_ ) );
    NET_ASN_Tools::ResetEnumeration( GetVariable( nDIAVerrouillageVisionIdx_ ) );
    NET_ASN_Tools::ResetPoint( visionPoint_, GetVariable( nDIAVisionPointIdx_ ) );
    NET_ASN_Tools::ResetDirection( visionDirection_, GetVariable( nDIAVisionDirectionIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::Initialize
// Created: 2005-6-28 - 14:3:4
// -----------------------------------------------------------------------------
bool MIL_PionMission_Test_MoveTo::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_Test_MoveTo& mission = static_cast< MIL_PionMission_Test_MoveTo& >( missionTmp );

    NET_ASN_Tools::CopyPath( mission.GetVariable( nDIAItineraireIdx_ ), itineraire_, GetVariable( nDIAItineraireIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIATypeItineraireIdx_ ), GetVariable( nDIATypeItineraireIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIADebarqueIdx_ ), GetVariable( nDIADebarqueIdx_ ) );
    NET_ASN_Tools::CopyEnumeration( mission.GetVariable( nDIAVerrouillageVisionIdx_ ), GetVariable( nDIAVerrouillageVisionIdx_ ) );
    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAVisionPointIdx_ ), visionPoint_, GetVariable( nDIAVisionPointIdx_ ) );
    NET_ASN_Tools::CopyDirection( mission.GetVariable( nDIAVisionDirectionIdx_ ), visionDirection_, GetVariable( nDIAVisionDirectionIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::Terminate
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_MoveTo::Terminate()
{
    NET_ASN_Tools::ResetDirection( visionDirection_, GetVariable( nDIAVisionDirectionIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::Serialize
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_MoveTo::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *new ASN1T_Mission_Pion_Test_MoveTo();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_test_move_to;
    asnMsg.mission.u.mission_pion_test_move_to  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIATypeItineraireIdx_ ), asnMission.type_itineraire );
    NET_ASN_Tools::CopyBool( GetVariable( nDIADebarqueIdx_ ), asnMission.debarque );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAVerrouillageVisionIdx_ ), asnMission.verrouillage_vision );
    NET_ASN_Tools::CopyPoint( GetVariable( nDIAVisionPointIdx_ ), asnMission.vision_point );
    NET_ASN_Tools::CopyDirection( GetVariable( nDIAVisionDirectionIdx_ ), asnMission.vision_direction );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Test_MoveTo::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
void MIL_PionMission_Test_MoveTo::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_test_move_to );
    ASN1T_Mission_Pion_Test_MoveTo& asnMission = *asnMsg.mission.u.mission_pion_test_move_to;

    NET_ASN_Tools::Delete( asnMission.itineraire );
    NET_ASN_Tools::Delete( asnMission.vision_point );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
