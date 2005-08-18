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
#include "MIL_PionMission_SeFaireTransporter.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_SeFaireTransporter::nDIAPointEmbarquementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::InitializeDIA
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_SeFaireTransporter::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "pointEmbarquement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter constructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_SeFaireTransporter::MIL_PionMission_SeFaireTransporter( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter destructor
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_PionMission_SeFaireTransporter::~MIL_PionMission_SeFaireTransporter()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_SeFaireTransporter::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *asnMsg.mission.u.mission_pion_se_faire_transporter;
    if( !NET_ASN_Tools::CopyPoint( asnMission.point_embarquement, pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::Initialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
bool MIL_PionMission_SeFaireTransporter::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    NET_ASN_Tools::ResetPoint( pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) );

    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::Initialize
// Created: 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
bool MIL_PionMission_SeFaireTransporter::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_SeFaireTransporter& mission = static_cast< MIL_PionMission_SeFaireTransporter& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIAPointEmbarquementIdx_ ), pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::Terminate
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_SeFaireTransporter::Terminate()
{
    NET_ASN_Tools::ResetPoint( pointEmbarquement_, GetVariable( nDIAPointEmbarquementIdx_ ) );

    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::Serialize
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_SeFaireTransporter::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *new ASN1T_Mission_Pion_SeFaireTransporter();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_se_faire_transporter;
    asnMsg.mission.u.mission_pion_se_faire_transporter  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIAPointEmbarquementIdx_ ), asnMission.point_embarquement );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_SeFaireTransporter::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
void MIL_PionMission_SeFaireTransporter::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_se_faire_transporter );
    ASN1T_Mission_Pion_SeFaireTransporter& asnMission = *asnMsg.mission.u.mission_pion_se_faire_transporter;

    NET_ASN_Tools::Delete( asnMission.point_embarquement );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
