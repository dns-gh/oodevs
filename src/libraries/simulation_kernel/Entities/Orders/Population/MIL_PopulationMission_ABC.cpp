//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Population/MIL_PopulationMission_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:16 $
// $Revision: 7 $
// $Workfile: MIL_PopulationMission_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PopulationMission_ABC.h"

#include "MIL_PopulationMissionType.h"

#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Decision/DEC_Tools.h"

int MIL_PopulationMission_ABC::nDIAMissionType_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_PopulationMission_ABC::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Mission_Population" );
    nDIAMissionType_ = DEC_Tools::InitializeDIAField( "missionType_"    , diaType );
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC constructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_PopulationMission_ABC::MIL_PopulationMission_ABC( MIL_Population& population, const MIL_PopulationMissionType& type )
    : DIA_Thing             ( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( type.GetDIATypeName() ) )
    , population_           ( population )
    , type_                 ( type     )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIAMissionType_ ).SetValue( (int)type.GetID() );
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC destructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_PopulationMission_ABC::~MIL_PopulationMission_ABC()
{
}

//=============================================================================
// INIT
//=============================================================================
  
//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::Initialize
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PopulationMission_ABC::Initialize( const ASN1T_MsgPopulationOrder& asnMsg )
{
    nOrderID_ = asnMsg.order_id;
    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::Terminate
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_PopulationMission_ABC::Terminate()
{
    nOrderID_  = (uint)-1;
    assert( !bDIABehaviorActivated_ );
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::Start
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_PopulationMission_ABC::Start()
{
    if( bDIABehaviorActivated_ )
        return;
    population_.GetDecision().StartMissionBehavior( *this );
    bDIABehaviorActivated_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::Stop
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_PopulationMission_ABC::Stop()
{
    if( !bDIABehaviorActivated_ )
        return;
    population_.GetDecision().StopMissionBehavior( *this );
    bDIABehaviorActivated_ = false;
}

// =============================================================================
// MISC
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::GetName
// Created: JVT 02-09-05
//-----------------------------------------------------------------------------
const char* MIL_PopulationMission_ABC::GetName() const
{
    return type_.GetBehaviorName().c_str();
}
