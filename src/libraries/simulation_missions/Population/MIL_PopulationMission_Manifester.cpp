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

#include "simulation_missions_pch.h"
#include "MIL_PopulationMission_Manifester.h"

#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationOrderManager.h"
#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

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
    if( !NET_ASN_Tools::CopyPoint( asnMission.destination, GetVariable( nDIADestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
