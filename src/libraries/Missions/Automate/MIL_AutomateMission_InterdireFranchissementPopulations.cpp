// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_InterdireFranchissementPopulations.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_InterdireFranchissementPopulations::nDIAPointsIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_InterdireFranchissementPopulations::MIL_AutomateMission_InterdireFranchissementPopulations( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_InterdireFranchissementPopulations::~MIL_AutomateMission_InterdireFranchissementPopulations()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_InterdireFranchissementPopulations::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointsIdx_ = DEC_Tools::InitializeDIAField( "points_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_InterdireFranchissementPopulations::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_InterdireFranchissementPopulations& asnMission = *asnMsg.mission.u.mission_automate_interdire_franchissement_populations;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points, GetVariable( nDIAPointsIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_InterdireFranchissementPopulations::Terminate()
{
    
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_InterdireFranchissementPopulations::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_InterdireFranchissementPopulations& asnMission = *new ASN1T_Mission_Automate_InterdireFranchissementPopulations();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_interdire_franchissement_populations;
    asnMsg.mission.u.mission_automate_interdire_franchissement_populations  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsIdx_ ), asnMission.points );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_InterdireFranchissementPopulations::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_InterdireFranchissementPopulations::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_interdire_franchissement_populations );
    ASN1T_Mission_Automate_InterdireFranchissementPopulations& asnMission = *asnMsg.mission.u.mission_automate_interdire_franchissement_populations;

    NET_ASN_Tools::Delete( asnMission.points );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
