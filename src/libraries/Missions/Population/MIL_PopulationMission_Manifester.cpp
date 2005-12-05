// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPopulation_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPopulation_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"
#include "MIL_PopulationMission_Manifester.h"

#include "MIL/Entities/Orders/Population/MIL_PopulationOrderManager.h"
#include "MIL/Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "MIL/Entities/Populations/MIL_Population.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PopulationMission_Manifester::nDIADestinationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PopulationMission_Manifester::InitializeDIA( const MIL_PopulationMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIADestinationIdx_ = DEC_Tools::InitializeDIAField( "destination_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_Manifester::MIL_PopulationMission_Manifester( MIL_Population& population, const MIL_PopulationMissionType& type )
: MIL_PopulationMission_ABC( population, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_Manifester::~MIL_PopulationMission_Manifester()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PopulationMission_Manifester::Initialize( const ASN1T_MsgPopulationOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PopulationMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Population_Manifester& asnMission = *asnMsg.mission.u.mission_population_manifester;
    if( !NET_ASN_Tools::CopyPoint( asnMission.destination, destination_, GetVariable( nDIADestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

/*
// ------------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PopulationMission_Manifester::Initialize( MIL_PopulationMission_ABC& missionTmp )
{
    if( !MIL_PopulationMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PopulationMission_Manifester& mission = static_cast< MIL_PopulationMission_Manifester& >( missionTmp );

    NET_ASN_Tools::CopyPoint( mission.GetVariable( nDIADestinationIdx_ ), destination_, GetVariable( nDIADestinationIdx_ ) );

    return true;
}                                                                    
*/
//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_Manifester::Terminate()
{
    NET_ASN_Tools::ResetPoint( destination_, GetVariable( nDIADestinationIdx_ ) );

    MIL_PopulationMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

/*
//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_Manifester::Serialize( ASN1T_MsgPopulationOrder& asnMsg )
{
    MIL_PopulationMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Population_Manifester& asnMission = *new ASN1T_Mission_Population_Manifester();
    asnMsg.mission.t                           = T_Mission_Population_mission_population_manifester;
    asnMsg.mission.u.mission_population_manifester  = &asnMission;

    NET_ASN_Tools::CopyPoint( GetVariable( nDIADestinationIdx_ ), asnMission.destination );

}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_Manifester::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PopulationMission_Manifester::CleanAfterSerialization( ASN1T_MsgPopulationOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Population_mission_population_manifester );
    ASN1T_Mission_Population_Manifester& asnMission = *asnMsg.mission.u.mission_population_manifester;

    NET_ASN_Tools::Delete( asnMission.destination );

    delete &asnMission;

    MIL_PopulationMission_ABC::CleanAfterSerialization( asnMsg );
}
*/