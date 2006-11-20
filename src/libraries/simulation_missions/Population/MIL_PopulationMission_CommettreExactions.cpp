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
#include "MIL_PopulationMission_CommettreExactions.h"

#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationOrderManager.h"
#include "simulation_kernel/Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PopulationMission_CommettreExactions::nDIAZoneIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_CommettreExactions::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PopulationMission_CommettreExactions::InitializeDIA( const MIL_PopulationMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_CommettreExactions constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_CommettreExactions::MIL_PopulationMission_CommettreExactions( MIL_Population& population, const MIL_PopulationMissionType& type )
: MIL_PopulationMission_ABC( population, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_CommettreExactions destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PopulationMission_CommettreExactions::~MIL_PopulationMission_CommettreExactions()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_CommettreExactions::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PopulationMission_CommettreExactions::Initialize( const ASN1T_MsgPopulationOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PopulationMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Population_CommettreExactions& asnMission = *asnMsg.mission.u.mission_population_commettre_exactions;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
