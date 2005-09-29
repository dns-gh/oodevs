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
#include "MIL_PopulationMission_FaireMouvement.h"

//#include "MIL/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "MIL/Entities/Orders/Population/MIL_PopulationMissionType.h"
//#include "MIL/Entities/Agents/MIL_AgentPion.h"
//#include "MIL/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
//#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Network/NET_ASN_Types.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_PopulationMission_FaireMouvement::nDIADestinationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_FaireMouvement::InitializeDIA
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_PopulationMission_FaireMouvement::InitializeDIA( const MIL_PopulationMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIADestinationIdx_ = DEC_Tools::InitializeDIAField( "destination_", diaType );
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_FaireMouvement constructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PopulationMission_FaireMouvement::MIL_PopulationMission_FaireMouvement( MIL_Population& population, const MIL_PopulationMissionType& type )
: MIL_PopulationMission_ABC( population, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_FaireMouvement destructor
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_PopulationMission_FaireMouvement::~MIL_PopulationMission_FaireMouvement()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_FaireMouvement::Initialize
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PopulationMission_FaireMouvement::Initialize( const ASN1T_MsgPopulationOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PopulationMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Population_FaireMouvement& asnMission = *asnMsg.mission.u.mission_population_faire_mouvement;
    if( !NET_ASN_Tools::CopyPoint( asnMission.destination, destination_, GetVariable( nDIADestinationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_FaireMouvement::Terminate
// Created: 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
void MIL_PopulationMission_FaireMouvement::Terminate()
{
    MIL_PopulationMission_ABC::Terminate();    
}
