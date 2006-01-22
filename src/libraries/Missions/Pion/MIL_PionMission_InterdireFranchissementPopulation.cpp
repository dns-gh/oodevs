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

#include "Missions_pch.h"
#include "MIL_PionMission_InterdireFranchissementPopulation.h"

#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "MIL/Entities/Agents/MIL_AgentPion.h"
#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PionMission_InterdireFranchissementPopulation::nDIALigneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_InterdireFranchissementPopulation::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIALigneIdx_ = DEC_Tools::InitializeDIAField( "ligne_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_InterdireFranchissementPopulation::MIL_PionMission_InterdireFranchissementPopulation( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_InterdireFranchissementPopulation::~MIL_PionMission_InterdireFranchissementPopulation()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_InterdireFranchissementPopulation::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_InterdireFranchissementPopulation& asnMission = *asnMsg.mission.u.mission_pion_interdire_franchissement_population;
    if( !NET_ASN_Tools::CopyLocation( asnMission.ligne, GetVariable( nDIALigneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_InterdireFranchissementPopulation::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

    
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_InterdireFranchissementPopulation::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_InterdireFranchissementPopulation& mission = static_cast< MIL_PionMission_InterdireFranchissementPopulation& >( missionTmp );

    NET_ASN_Tools::CopyLocation( mission.GetVariable( nDIALigneIdx_ ), GetVariable( nDIALigneIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulation::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulation::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_InterdireFranchissementPopulation& asnMission = *new ASN1T_Mission_Pion_InterdireFranchissementPopulation();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_interdire_franchissement_population;
    asnMsg.mission.u.mission_pion_interdire_franchissement_population  = &asnMission;

    NET_ASN_Tools::CopyLocation( GetVariable( nDIALigneIdx_ ), asnMission.ligne );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_InterdireFranchissementPopulation::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_InterdireFranchissementPopulation::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_interdire_franchissement_population );
    ASN1T_Mission_Pion_InterdireFranchissementPopulation& asnMission = *asnMsg.mission.u.mission_pion_interdire_franchissement_population;

    NET_ASN_Tools::Delete( asnMission.ligne );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
